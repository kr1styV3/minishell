/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:05:01 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 20:07:35 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "state.h"
#include "parsing.h"
#include "builtins.h"
#include "read_line.h"
#include "env_variables.h"

#define OPERATORS "<>|"

static inline char	get_cchar(t_tokenizer_ctx *ctx)
{
	return (ctx->str[ctx->pos]);
}

static void	handle_normal(t_tokenizer_ctx *ctx)
{
	if (ft_isalnum(get_cchar(ctx)))
		ctx->pos += process_token(&ctx->token, ctx->str, ctx->pos, &ctx->state);
	if (check_var(&(ctx->token), ctx->str, &ctx->pos, &(ctx->env)) == 0)
	{
		ctx->token->env_work = true;
		ctx->token->exec = false;
	}
	ctx->pos += ft_checkwordarg(&ctx->token, ctx->str, ctx->pos);
	ctx->pos += skip_whitespaces(&ctx->str[ctx->pos], &ctx->state);
	if (!ctx->token->arg && ctx->token->checker == true
		&& !ft_isbuiltin(ctx->token->parsed->token))
		ctx->token->arg = (char **)ft_calloc(2, sizeof(char *));
	if (ft_isbuiltin(ctx->token->parsed->token))
	{
		ctx->state = IN_BUILTIN;
		return ;
	}
	update_state_from_char(ctx);
}

void	update_state_from_char(t_tokenizer_ctx *ctx)
{
	if (ctx->str[ctx->pos] == 34 || ctx->str[ctx->pos] == 39)
		ctx->state = IN_WORD;
	else if (ft_strchr(OPERATORS, ctx->str[ctx->pos]))
		ctx->state = IN_OPERATOR;
	else if (ft_isbuiltin(ctx->token->parsed->token))
		ctx->state = IN_BUILTIN;
	else if (ctx->str[ctx->pos] == '$' && ctx->state != IN_BUILTIN)
		ctx->state = IN_VARIABLE;
}

static void	process_state(t_tokenizer_ctx *ctx)
{
	if (ctx->state == SKIP_WHITESPACE)
		handle_whitespace(ctx);
	else if (ctx->state == IN_OPERATOR)
	{
		ctx->pos += process_operator(&ctx->token, ctx->str,
				ctx->pos, &ctx->state);
		if (ctx->token->operator->operator == '|')
			ctx->next_token = true;
	}
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
			if ((ctx.state != IN_BUILTIN && token->operator->operator == '|')
				|| (get_cchar(&ctx) == '\0' || g_should_exit))
				return ;
		}
		next = init_token();
		ctx.token->next = next;
		ctx.token = ctx.token->next;
		ctx.state = SKIP_WHITESPACE;
		ctx.next_token = false;
	}
}
