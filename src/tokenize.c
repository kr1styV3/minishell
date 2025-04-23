/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:05:01 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/14 16:18:47 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "state.h"
#include "parsing.h"
#include "builtins.h"
#include "read_line.h"
#include "env_variables.h"

#define OPERATORS "<>|"

static void	handle_normal(t_tokenizer_ctx *ctx)
{
	t_bau_args	tokenize_sto_norminette_del_caizer;

	tokenize_sto_norminette_del_caizer.bau_bau = ctx->envp;
	tokenize_sto_norminette_del_caizer.meow = ctx->env;
	if (ft_isalnum(ctx->str[ctx->pos]) || ctx->str[ctx->pos] == '/')
		ctx->pos += process_token(&ctx->token, ctx->str, ctx->pos, &ctx->state);
	if (!ctx->token->parsed->token && ctx->str[ctx->pos] == '.')
		ctx->pos += process_file_cmd(ctx->token, ctx->str, ctx->pos);
	if (ctx->token->parsed->token)
		if (access(ctx->token->parsed->token, X_OK) == 0)
			ctx->token->checker = false;
	if (check_var(&(ctx->token), ctx->str, &ctx->pos, &(ctx->env)) == 0)
		ctx->token->exec = false;
	ctx->pos += ft_checkwordarg(&ctx->token,
			ctx->str, ctx->pos, &tokenize_sto_norminette_del_caizer);
	if (ctx->str[0] == '/' && ctx->token->checker == false && ctx->token->arg)
		ctx->token->arg[0] = ft_strdup(ctx->token->parsed->token);
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
	else if (ctx->str[ctx->pos] == '\0')
		ctx->state = NORMAL;
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

static t_token	*tokenizer_next_token_alloc(t_token **token)
{
	t_token	*next_token;

	next_token = init_token();
	if (!next_token)
		return (free_inside_token("failed to allocate memory :",
				"next_token"), (t_token *) NULL);
	(*token)->next = next_token;
	(*token) = (*token)->next;
	return (next_token);
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
	while (ctx.str[ctx.pos])
	{
		while (!ctx.next_token)
		{
			process_state(&ctx);
			if (token->operator->operator == '|')
				break ;
			if ((ctx.state != IN_BUILTIN || g_should_exit)
				&& (ctx.str[ctx.pos] == '\0' || g_should_exit))
				return ;
		}
		next = tokenizer_next_token_alloc(&token);
		if (!next)
			token->exec = false;
		ctx.next_token = false;
		ctx.token = token;
	}
}
