/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:42:59 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/14 14:22:00 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_token.h"
#include <stdlib.h>

void	handle_token_error(t_token *token,
	t_parse *parsed, t_operator *op, t_doc *doc)
{
	if (parsed)
		free(parsed);
	if (op)
		free(op);
	if (doc)
		free(doc);
	if (token)
		free(token);
}

void	set_token_defaults(t_token *token,
	t_parse *parsed, t_operator *op, t_doc *doc)
{
	token->exec = true;
	token->env_work = false;
	token->free = false;
	token->checker = false;
	token->last_exit_status = -1;
	token->arg = NULL;
	token->parsed = parsed;
	token->operator = op;
	token->doc = doc;
	token->env = NULL;
	token->next = NULL;
}
