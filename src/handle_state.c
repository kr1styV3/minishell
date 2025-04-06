/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:22:44 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/04 20:00:22 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"
#include "minishell.h"
#include "t_token.h"
#include "parsing.h"
#include "env_variables.h"
#include "builtins.h"
#include <stdbool.h>

void	handle_whitespace(t_tokenizer_ctx *ctx)
{
	ctx->pos += skip_whitespaces(&ctx->str[ctx->pos], &ctx->state);
}

void	handle_builtin(t_tokenizer_ctx *ctx)
{
	ctx->pos += process_builtin(&ctx->token, ctx->str, ctx->pos,
			&ctx->state, ctx->env, ctx->envp);
	ctx->pos += skip_whitespaces(&ctx->str[ctx->pos], &ctx->state);
	update_state_from_char(ctx);
}

void	handle_variable(t_tokenizer_ctx *ctx)
{
	ctx->pos += process_variable(&ctx->token, ctx->str,
			ctx->pos + 1, ctx->env, ctx->envp) + 1;
}

void	handle_word(t_tokenizer_ctx *ctx)
{
	ctx->pos += process_word(&ctx->token, ctx->str, ctx->pos,
			&ctx->state, ctx->env);
}
