/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:10:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/02 16:24:32 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"
#include "t_token.h"

#include "operators_utils.h"

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
		return (free(file), free_tokens_line(str, token, "Failed to open file"), -1);
	free(file);
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
		return (free(file), free_tokens_line(str, token, "Failed to open file"), -1);
	free(file);
	return (length);
}

int	here_doc_init(t_token *token, char *str, int i)
{
	int		length;
	char	*delimiter;
	t_doc	*next_doc;

	length = skip_whitespaces(&str[i], NULL);
	delimiter = extract_file_token(&str[i + length]);
	if (!delimiter)
		return (free_tokens_line(str, token, "malloc error"), -1);
	length += ft_strlen(delimiter);
	if (token->doc->here_doc == true)
	{
		next_doc = (t_doc *)malloc(sizeof(t_doc));
		next_doc->here_doc = true;
		next_doc->eof = ft_strdup(delimiter);
		if (token->doc->next)
			token->doc = token->doc->next;
		token->doc->next = next_doc;
		next_doc->next = NULL;
	}
	else
	{
		token->doc->here_doc = true;
		token->doc->eof = ft_strdup(delimiter);
		(*token->op) = token->doc;
	}
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
		return (free(file), free_tokens_line(str, token, "Failed to open file"), -1);
	free(file);
	return (length);
}

int	process_operator(t_token **token, char *str, int pos, t_state *state)
{
	int	consumed;

	consumed = 0;
	while (str[pos + consumed] != '\0' && str[pos + consumed] != '|')
	{
		if (str[pos + consumed] == '>' && str[pos + consumed + 1] == '>')
			consumed += check_append_fd(*token, str, pos + consumed + 2) + 2;
		else if (str[pos + consumed] == '>')
			consumed += check_overwrite_fd(*token, str, pos + consumed + 1) + 1;
		else if (str[pos + consumed] == '<' && str[pos + consumed + 1] == '<')
			consumed += here_doc_init(*token, str, pos + consumed + 2) + 2;
		else if (str[pos + consumed] == '<')
			consumed += input_from_file(*token, str, pos + consumed + 1) + 1;
		else if (str[pos + consumed] == '.' && str[pos + consumed + 1] == '/')
			consumed += process_file_cmd(*token, str, pos + consumed);
		else if (str[pos + consumed] == '-')
			consumed += parse_flags(*token, str, pos + consumed);
		else
			break ;
	}
	if (str[pos + consumed++] == '|')
		(*token)->operator->operator = '|';
	if (state)
		*state = SKIP_WHITESPACE;
	return (consumed);
}
