/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:22:44 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/01 13:06:08 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"
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

void	handle_normal(t_tokenizer_ctx *ctx)
{
	if (ft_isalnum(get_cchar(ctx)))
		ctx->pos += process_token(&ctx->token, ctx->str, ctx->pos, &ctx->state);
	if (check_var(&ctx->token, ctx->str, &ctx->pos, &ctx->env) == 0)
	{
		ctx->token->env_work = true;
		ctx->token->exec = false;
	}
	ctx->pos += ft_checkwordarg(&ctx->token, ctx->str, ctx->pos);
	ctx->pos += skip_whitespaces(&ctx->str[ctx->pos], &ctx->state);
	if (!ctx->token->arg && ctx->token->checker == true
		&& !ft_isbuiltin(ctx->token->parsed->token))
		ctx->token->arg = (char **)ft_calloc(2, sizeof(char *));
	update_state_from_char(ctx);
}
