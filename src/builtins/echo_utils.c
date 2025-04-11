/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:59:25 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/11 17:48:15 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/builtins.h"
#include "../../headers/parsing.h"
#include "../../headers/env_variables.h"
#include "t_token.h"

int	build_echo_arguments(t_token *token,
		char *output, bool flag, char *str)
{
	token->arg = ft_calloc(3, sizeof(char *));
	if (!token->arg)
		return (free_tokens_line(str, token, "malloc error"), -1);
	token->arg[0] = ft_strdup("echo");
	if (!token->arg[0])
		return (free_tokens_line(str, token, "malloc error"), -1);
	token->arg[1] = ft_strdup(output);
	if (!token->arg[1])
		return (free_tokens_line(str, token, "malloc error"), -1);
	if (!flag)
	{
		token->arg[1] = ft_freejoin(token->arg[1], "\n");
		if (!token->arg[1])
			return (free_tokens_line(str, token, "malloc error"), -1);
	}
	token->arg[2] = NULL;
	token->checker = false;
	return (0);
}

int	handle_plain_text(t_echo_ctx *ctx)
{
	int		start;
	int		j;
	int		substr_len;
	char	*substr;

	start = ctx->i + ctx->len;
	j = start;
	while (ctx->str[j] && ctx->str[j] != '$'
		&& ctx->str[j] != '\'' && ctx->str[j] != '\"'
		&& ctx->str[j] != ' ')
		j++;
	substr_len = j - start;
	substr = ft_substr(ctx->str, start, substr_len);
	if (!substr || append_to_output(&ctx->output,
			substr, ctx->str, ctx->token) == -1)
		return (free(substr), -1);
	free(substr);
	ctx->len += substr_len;
	return (0);
}

int	handle_env_var(t_echo_ctx *ctx, t_bau_args *bau_norm)
{
	int	var_len;

	var_len = process_variable(&ctx->token, ctx->str,
			ctx->i + ctx->len + 1, bau_norm);
	if (var_len == -1)
		return (-1);
	ctx->len += var_len + 1;
	if (ctx->token->parsed->word)
	{
		if (append_to_output(&ctx->output,
				ctx->token->parsed->word, ctx->str, ctx->token) == -1)
			return (-1);
	}
	return (0);
}

int	handle_single_quote(t_echo_ctx *ctx)
{
	char	*word;

	word = extract_word(&ctx->str[ctx->i + ctx->len + 1], '\'');
	if (!word)
		return (free_tokens_line(ctx->str, ctx->token,
				"malloc error"), -1);
	ctx->len += ft_strlen(word) + 2;
	if (append_to_output(&ctx->output, word, ctx->str, ctx->token) == -1)
		return (free(word), -1);
	free(word);
	return (ctx->len);
}

int	append_to_output(char **output,
	char *addition, char *str, t_token *token)
{
	char	*temp;

	temp = ft_strjoin(*output, addition);
	free(*output);
	if (!temp)
		return (free_tokens_line(str, token, "malloc error"), -1);
	*output = temp;
	return (0);
}
