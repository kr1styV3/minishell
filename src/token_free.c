/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:51:23 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 20:54:45 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "t_token.h"

static void	close_operator_fds(t_operator *op)
{
	if (op->fd_append_output > 0)
		close(op->fd_append_output);
	if (op->fd_overwrite_output > 0)
		close(op->fd_overwrite_output);
	if (op->fd_input > 0)
		close(op->fd_input);
}

static void	free_doc_list(t_token *token)
{
	t_doc	*doc;
	t_doc	*next;

	doc = (*token->op);
	while (doc)
	{
		next = doc->next;
		if (doc->eof)
			free(doc->eof);
		free(doc);
		doc = next;
	}
	free(token->op);
}

static void	free_parsed(t_parse *parsed)
{
	if (parsed->token)
		free(parsed->token);
	if (parsed->word)
		free(parsed->word);
	free(parsed);
}

void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		if (token->parsed)
			free_parsed(token->parsed);
		if (token->arg)
			ft_free_mtx(token->arg);
		free_doc_list(token);
		close_operator_fds(token->operator);
		free(token->operator);
		free(token);
		token = tmp;
	}
}
