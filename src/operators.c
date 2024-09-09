/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:10:31 by chrlomba          #+#    #+#             */
/*   Updated: 2024/09/09 05:22:27 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"

int	check_append_fd(t_token *token, char *str, int string_position)
{
	int length;
	char *file;

	length = skip_whitespaces(&str[string_position], NULL);
	file = extract_token(&str[string_position + length]);
	if (!file)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(file);
	token->fd_append_output = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (token->fd_append_output < 0)
		return (free_tokens_line(str, token, "Failed to open file"), -1);
	return (length);
}

int	check_overwrite_fd(t_token *token, char *str, int string_position)
{
	int length;
	char *file;

	length = skip_whitespaces(&str[string_position], NULL);
	file = extract_token(&str[string_position + length]);
	if (!file)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(file);
	token->fd_overwrite_output = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (token->fd_overwrite_output < 0)
		return (free_tokens_line(str, token, "Failed to open file"), -1);
	return (length);
}

int	here_doc(t_token *token, char *str, int string_position)
{
	int length;
	char *delimiter;
	char *line;

	length = skip_whitespaces(&str[string_position], NULL);
	delimiter = extract_token(&str[string_position + length]);
	if (!delimiter)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (free_tokens_line(str, token, "Failed to read line"), -1);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		token->here_doc = ft_freejoin(token->here_doc, line);
		if (!token->here_doc)
			return (free_tokens_line(str, token, "malloc error"), -1);
	}
	return (length);
}

int	input_from_file(t_token *token, char *str, int string_position)
{
	int length;
	char *file;

	length = skip_whitespaces(&str[string_position], NULL);
	file = extract_token(&str[string_position + length]);
	if (!file)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(file);
	token->fd_input = open(file, O_RDONLY);
	if (token->fd_input < 0)
		return (free_tokens_line(str, token, "Failed to open file"), -1);
	return (length);
}

int process_operator(t_token *token, char *str, int string_position, t_state *state)
{
	int length;

	length = 0;
	token->operator = str[string_position];  // Save the operator (e.g., |, >, <).
	if (str[string_position] == '>' && str[string_position + 1] == '>')  // ">>" operator
		length = check_append_fd(token, str, string_position + 2) + 2;  // Check if the operator is ">>" and set the file descriptor.
	else if (str[string_position] == '>')  // ">" operator
		length = check_overwrite_fd(token, str, string_position + 1) + 1;// Initialize file path for output redirection
	else if (str[string_position] == '<' && str[string_position + 1] == '<')  // "<<" operator
		length = here_doc(token, str, string_position + 2) + 2;  // Check if the operator is "<<" and set the file descriptor.
	else if (str[0] == '<')  // "<" operator
		length = input_from_file(token, str, string_position + 1) + 1;  // Initialize file path for input redirection
	else if (str[0] == '|')  // "|" operator
		token->next_is_pipe = true;  // Set operator type
	else if (str[0] == '&')  // "&" operator
		token->background = true;  // Set background execution flag
	else
		*state = SKIP_WHITESPACE;  // Default case
	return length;
}
