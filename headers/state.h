/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:34:57 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/01 13:26:08 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
# define STATE_H

typedef enum s_state
{
	NORMAL,
	IN_BUILTIN,
	IN_WORD,
	IN_OPERATOR,
	IN_VARIABLE,
	SKIP_WHITESPACE,
	FREE_TOKEN
}	t_state;

typedef struct s_tokenizer_ctx
{
	char		*str;
	int			pos;
	t_token		*token;
	t_state		state;
	t_env_list	*env;
	char		**envp;
	bool		next_token;
}	t_tokenizer_ctx;

inline char	get_cchar(t_tokenizer_ctx *ctx)
{
	return (ctx->str[ctx->pos]);
}
void	update_state_from_char(t_tokenizer_ctx *ctx);
void	handle_normal(t_tokenizer_ctx *ctx);
void	handle_word(t_tokenizer_ctx *ctx);
void	handle_variable(t_tokenizer_ctx *ctx);
void	handle_builtin(t_tokenizer_ctx *ctx);
void	handle_whitespace(t_tokenizer_ctx *ctx);

#endif
