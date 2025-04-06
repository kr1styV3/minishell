/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:37:55 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 20:49:17 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "t_token.h"

static t_doc	*init_doc(void)
{
	t_doc	*doc;

	doc = malloc(sizeof(t_doc));
	if (!doc)
		return (NULL);
	doc->eof = NULL;
	doc->here_doc = false;
	doc->next = NULL;
	return (doc);
}

static t_operator	*init_operator(void)
{
	t_operator	*op;

	op = malloc(sizeof(t_operator));
	if (!op)
		return (NULL);
	op->fd_input = -1;
	op->fd_overwrite_output = -1;
	op->fd_append_output = -1;
	op->operator = 0;
	return (op);
}

static t_parse	*init_parsed(void)
{
	t_parse	*parsed;

	parsed = malloc(sizeof(t_parse));
	if (!parsed)
		return (NULL);
	parsed->token = NULL;
	parsed->word = NULL;
	return (parsed);
}

static t_doc	**init_doc_original_pointer(t_token *token,
			t_parse *parsed, t_operator *op, t_doc *doc)
{
	t_doc	**ptr;

	ptr = malloc(sizeof(t_doc *));
	if (!ptr)
	{
		handle_token_error(token, parsed, op, doc);
		ft_error("Failed to allocate doc pointer.");
		return (NULL);
	}
	return (ptr);
}

t_token	*init_token(void)
{
	t_token		*token;
	t_parse		*parsed;
	t_operator	*op;
	t_doc		*doc;
	t_doc		**op_ptr;

	token = malloc(sizeof(t_token));
	if (!token)
		return (ft_error("Failed to allocate memory for token."), NULL);
	parsed = init_parsed();
	op = init_operator();
	doc = init_doc();
	if (!parsed || !op || !doc)
	{
		handle_token_error(token, parsed, op, doc);
		ft_error("Failed to allocate token components.");
		return (NULL);
	}
	op_ptr = init_doc_original_pointer(token, parsed, op, doc);
	if (!op_ptr)
		return (NULL);
	*op_ptr = doc;
	set_token_defaults(token, parsed, op, doc);
	token->op = op_ptr;
	return (token);
}
