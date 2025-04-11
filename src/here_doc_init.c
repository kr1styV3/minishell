/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:21:36 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/10 16:33:26 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"
#include "t_token.h"

static t_doc	*create_doc_from_delimiter(char *delimiter)
{
	t_doc	*doc;

	doc = malloc(sizeof(t_doc));
	if (!doc)
		return (NULL);
	doc->here_doc = true;
	doc->eof = ft_strdup(delimiter);
	doc->next = NULL;
	return (doc);
}

static int	allocate_next_doc(t_token *token, char *delimiter, char *str)
{
	t_doc	*new_doc;

	new_doc = create_doc_from_delimiter(delimiter);
	if (!new_doc || !new_doc->eof)
	{
		if (new_doc)
			free(new_doc);
		return (free_tokens_line(str, token, "malloc error"), 0);
	}
	if (token->doc->next)
		token->doc = token->doc->next;
	token->doc->next = new_doc;
	return (1);
}

static int	set_first_doc(t_token *token, char *delimiter, char *str)
{
	token->doc->here_doc = true;
	token->doc->eof = ft_strdup(delimiter);
	if (!token->doc->eof)
		return (free_tokens_line(str, token, "malloc error"), 0);
	(*token->op) = token->doc;
	return (1);
}

int	here_doc_init(t_token *token, char *str, int i)
{
	char	*delimiter;
	int		length;
	int		success;

	length = skip_whitespaces(&str[i], NULL);
	delimiter = extract_file_token(&str[i + length]);
	if (!delimiter)
		return (free_tokens_line(str, token, "malloc error"), 0);
	length += ft_strlen(delimiter);
	if (token->doc->here_doc)
		success = allocate_next_doc(token, delimiter, str);
	else
		success = set_first_doc(token, delimiter, str);
	free(delimiter);
	if (!success)
		return (0);
	return (length);
}
