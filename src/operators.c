/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:10:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/01/05 11:16:53 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"
#include "t_token.h"

int	check_append_fd(t_token *token, char *str, int string_position)
{
	int length;
	char *file;

	length = skip_whitespaces(&str[string_position], NULL);
	file = extract_token(&str[string_position + length]);
	if (!file)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(file);
	token->operator->fd_append_output = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (token->operator->fd_append_output < 0)
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
	token->operator->fd_overwrite_output = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (token->operator->fd_overwrite_output < 0)
		return (free_tokens_line(str, token, "Failed to open file"), -1);
	return (length);
}

int	here_doc_init(t_token *token, char *str, int i)
{
	int		length;
	char	*delimiter;

	length = skip_whitespaces(&str[i], NULL);
	delimiter = extract_token(&str[i + length]);
	if (!delimiter)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(delimiter);
	token->here_doc = true;
	token->eof = ft_strdup(delimiter);
	if (!token->eof)
		return (free_tokens_line(str, token, "malloc error"), -1);
	free(delimiter);
	return (length);
}

int	input_from_file(t_token *token, char *str, int string_position)
{
	int		length;
	char	*file;

	length = skip_whitespaces(&str[string_position], NULL);
	file = extract_token(&str[string_position + length]);
	if (!file)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(file);
	token->operator->fd_input = open(file, O_RDONLY);
	if (token->operator->fd_input < 0)
		return (free_tokens_line(str, token, "Failed to open file"), -1);
	return (length);
}

int process_operator(t_token **token, char *str, int string_position, t_state *state)
{
	int length;

	length = 1;
	(*token)->operator->operator = str[string_position];  // Save the operator (e.g., |, >, <).
	if (str[string_position] == '>' && str[string_position + 1] == '>')  // ">>" operator
		length = check_append_fd(*token, str, string_position + 2) + 2;  // Check if the operator is ">>" and set the file descriptor.
	else if (str[string_position] == '>')  // ">" operator
		length = check_overwrite_fd(*token, str, string_position + 1) + 1;// Initialize file path for output redirection
	else if (str[string_position] == '<' && str[string_position + 1] == '<')  // "<<" operator
		 length = here_doc_init(*token, str, string_position + 2) + 2;  // Initialize here-doc delimiter
	else if (str[string_position] == '<')  // "<" operator
		length = input_from_file(*token, str, string_position + 1) + 1;  // Initialize file path for input redirection
	else if (str[string_position] == '|')
	{
		(*token)->operator->operator = '|';
		return (length); // Set operator type
	}
	else if (str[string_position] == '&')
	{
		(*token)->operator->operator = '&';
		return (length); //Set background execution flag
	}
	else if (str[string_position] == '-')
		length = parse_flags(*token, str, string_position);  // Parse flags
	if (state)
		*state = SKIP_WHITESPACE;  // Default case
	return length;
}
