/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:05:01 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/01 13:14:08 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "state.h"
#include "parsing.h"

#define OPERATORS "<>|"

void	update_state_from_char(t_tokenizer_ctx *ctx)
{
	if (CUR_CHAR == 34 || CUR_CHAR == 39)
		ctx->state = IN_WORD;
	else if (ft_strchr(OPERATORS, CUR_CHAR))
		ctx->state = IN_OPERATOR;
	else if (ft_isbuiltin(ctx->token->parsed->token))
		ctx->state = IN_BUILTIN;
	else if (CUR_CHAR == '$' && ctx->state != IN_BUILTIN)
		ctx->state = IN_VARIABLE;
}

static void	process_state(t_tokenizer_ctx *ctx)
{
	if (ctx->state == SKIP_WHITESPACE)
		handle_whitespace(ctx);
	else if (ctx->state == IN_OPERATOR)
		ctx->pos += process_operator(&ctx->token, ctx->str,
				ctx->pos, ctx->state);
	else if (ctx->state == NORMAL)
		handle_normal(ctx);
	else if (ctx->state == IN_WORD)
		handle_word(ctx);
	else if (ctx->state == IN_VARIABLE)
		handle_variable(ctx);
	else if (ctx->state == IN_BUILTIN)
		handle_builtin(ctx);
}

void	tokenizer(char *str, t_token *token, t_env_list *env, char **envp)
{
	t_tokenizer_ctx	ctx;
	t_token			*next;

	ctx.str = str;
	ctx.pos = 0;
	ctx.token = token;
	ctx.env = env;
	ctx.envp = envp;
	ctx.state = SKIP_WHITESPACE;
	ctx.next_token = false;
	while (get_cchar(&ctx))
	{
		while (!ctx.next_token)
		{
			process_state(&ctx);
			if (get_cchar(&ctx) == '\0' || should_exit)
				return ;
		}
		next = init_token();
		ctx.token->next = next;
		ctx.token = ctx.token->next;
		ctx.state = SKIP_WHITESPACE;
		ctx.next_token = false;
	}
}
