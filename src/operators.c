/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:10:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/17 15:17:11 by chrlomba         ###   ########.fr       */
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
	file = extract_file_token(&str[string_position + length]);
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
	file = extract_file_token(&str[string_position + length]);
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
	delimiter = extract_file_token(&str[i + length]);
	if (!delimiter)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(delimiter);
	token->doc->here_doc = true;
	token->doc->eof = ft_strdup(delimiter);
	if (!token->doc->eof)
		return (free_tokens_line(str, token, "malloc error"), -1);
	free(delimiter);
	return (length);
}

int	input_from_file(t_token *token, char *str, int string_position)
{
	int		length;
	char	*file;

	length = skip_whitespaces(&str[string_position], NULL);
	file = extract_file_token(&str[string_position + length]);
	if (!file)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(file);
	token->operator->fd_input = open(file, O_RDONLY);
	if (token->operator->fd_input < 0)
		return (free_tokens_line(str, token, "Failed to open file"), -1);
	return (length);
}

char *extract_bash_file(char *str)
{
    int i = 0;
    char *token;

    if (!str)
        return NULL;

    // If the file name is enclosed in quotes
    if (str[i] == '\"')
    {
        i++; // Skip the opening quote
        int start = i;
        while (str[i] && str[i] != '\"')
            i++;
        // i now points to the closing quote (or end of string if no closing quote found)
        token = ft_calloc(i - start + 1, sizeof(char));
        if (!token)
            return NULL;
        ft_strncpy(token, &str[start], i - start);
        return token;
    }
    else
    {
        // Allow alnum characters as well as '.', '_', and '-' for file names
        while (str[i] && (ft_isalnum(str[i]) || str[i] == '.' || str[i] == '/'))
            i++;
        token = ft_calloc(i + 1, sizeof(char));
        if (!token)
            return NULL;
        ft_strncpy(token, str, i);
        return token;
    }
}

int	process_file_cmd(t_token *token, char *str, int i)
{
	int		length;
	char	*file;

	length = i;
	file = extract_bash_file(&str[length]);
	if (!file)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(file);
	token->arg[0] = ft_strdup(file);
	if (!token->arg[0])
		return (free_tokens_line(str, token, "malloc error"), -1);
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
		length = input_from_file(*token, str, string_position + 1) + 1;
	else if (str[string_position] == '.' && str[string_position + 1] == '/')
	{
		length = process_file_cmd(*token, str, string_position);
	}
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
