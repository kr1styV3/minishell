/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_handle_state.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:22:44 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/11 20:07:11 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"
#include "minishell.h"
#include "t_token.h"
#include "parsing.h"
#include "env_variables.h"
#include "builtins.h"
#include "executor.h"
#include <stdbool.h>

void	handle_whitespace(t_tokenizer_ctx *ctx)
{
	ctx->pos += skip_whitespaces(&ctx->str[ctx->pos], &ctx->state);
}

void	handle_builtin(t_tokenizer_ctx *ctx)
{
	t_bau_args	bau;

	bau.bau_bau = ctx->envp;
	bau.meow = ctx->env;
	ctx->pos += process_builtin(&ctx->token, ctx->str, ctx->pos,
			&bau);
	ctx->pos += skip_whitespaces(&ctx->str[ctx->pos], &ctx->state);
	update_state_from_char(ctx);
	if (ctx->state == IN_BUILTIN)
		ctx->state = NORMAL;
}

void	handle_variable(t_tokenizer_ctx *ctx)
{
	t_bau_args	meow_diobono_sto_norminette_del_caizer;

	meow_diobono_sto_norminette_del_caizer.meow = ctx->env;
	meow_diobono_sto_norminette_del_caizer.bau_bau = ctx->envp;
	ctx->pos += process_variable(&ctx->token, ctx->str,
			ctx->pos + 1, &meow_diobono_sto_norminette_del_caizer) + 1;
}

void	handle_word(t_tokenizer_ctx *ctx)
{
	t_bau_args	meow_diobono_sto_norminette_del_caizer;

	meow_diobono_sto_norminette_del_caizer.meow = ctx->env;
	meow_diobono_sto_norminette_del_caizer.bau_bau = ctx->envp;
	ctx->pos += process_word(&ctx->token, ctx->str, ctx->pos,
			&meow_diobono_sto_norminette_del_caizer);
}
