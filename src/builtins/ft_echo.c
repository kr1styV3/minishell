/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:50:32 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/11 20:04:07 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/builtins.h"
#include "../../headers/parsing.h"
#include "../../headers/env_variables.h"
#include "t_token.h"

// int ft_echo(t_token *token, char *str, int i, t_env_list *env, char **envp)
// {
//     int len = skip_whitespaces(&str[i], NULL);
//     bool flag = false;
//     char *word = NULL;
//     char quote;
//     char *output = ft_strdup(""); // Initialize an empty string for output
//     if (!output)
//         return (free_tokens_line(str, token, "malloc error"), -1);

//     // Handle '-n' flag
//     if (str[i + len] == '-' && str[i + len + 1] == 'n')
//     {
//         flag = true;
//         len += 2;
//     }

//     len += skip_whitespaces(&str[i + len], NULL);
//     quote = str[i + len];

//     // Handle different cases based on the first character after options
//     if (quote == '\'')
//     {
//         word = extract_word(&str[i + len + 1], quote);
//         if (!word)
//         {
//             free(output);
//             return (free_tokens_line(str, token, "malloc error"), -1);
//         }
//         len += ft_strlen(word) + 2; // Account for quotes
//         char *temp = ft_strjoin(output, word);
//         free(output);
//         free(word);
//         if (!temp)
//             return (free_tokens_line(str, token, "malloc error"), -1);
//         output = temp;
//     }
//     else if (quote == '\"')
//     {
//         word = extract_word_with_dollasign(&str[i
//         if (!word)
//         {
//             free(output);
//             return (free_tokens_line(str, token, "malloc error"), -1);
//         }
//         len += ft_strlen(word) + 2; // Account for quotes
//         char *temp = ft_strjoin(output, word);
//         free(output);
//         free(word);
//         if (!temp)
//             return (free_tokens_line(str, token, "malloc error"), -1);
//         output = temp;
//     }
//     else
//     {
//         // Handle unquoted strings, including those with multiple variables
//         while (str[i + len] && str[i + len] != '\'' &&
//         {
//             if (str[i + len] == '$')
//             {
//                 // Process variable
//                 int var_len = process_variable(&token,  + 1, env, envp);
//                 if (var_len == -1)
//                 {
//                     free(output);
//                     return (free_tokens_line(str, sing error"), -1);
//                 }
//                 len += var_len + 1; // +1 for '$'
//                 if (token->parsed->word)
//                 {
//                     char *temp = ft_strjoin(output, token->parsed->word);
//                     free(output);
//                     // free (token->parsed->word);
//                     if (!temp)
//                         return (free_tokens_line
//                     output = temp;
//                 }
//             }
//             else
//             {
//                 // Process literal characters
//                 int start = i + len;
//                 int j = start;
//                 while (str[j] && str[j] != '$' && s && str[j] != 32)
//                     j++;
//                 int substr_len = j - start;
//                 char *substr = ft_substr(str, start, substr_len);
//                 if (!substr)
//                 {
//                     free(output);
//                     return (free_token), -1);
//                 }
//                 len += substr_len;
//                 char *temp = ft_strjoin(output, substr);
//                 free(substr);
//                 free(output);
//                 if (!temp)
//                     return (free_tokens_line
//             }
//         }
//     }
//         token->arg = (char **)ft_calloc(3, sizeof(char *));
//         token->arg[0] = ft_strdup("echo");
//         if (!token->arg[0]));
//         token->arg[1] = ft_strdup(output);
//         if (!flag)
//             token->arg[1] = ft_freejoin(token->arg[1], "\n");
//         if (!token->arg[1])
//             return (free_tokens_line(str, token, "malloc
//         token->arg[2] = NULL;
//         token->checker = false;
//         free(output);
//     return len;
// }
static int	handle_double_quote(t_echo_ctx *ctx, t_bau_args *bau_norm_bau)
{
	char	*word;

	word = extract_word_with_dollasign(&ctx->str[ctx->i + ctx->len + 1],
			'\"', ctx->token, bau_norm_bau);
	if (!word)
		return (free_tokens_line(ctx->str, ctx->token,
				"malloc error"), -1);
	ctx->len += ft_strlen(word) + 2;
	if (append_to_output(&ctx->output, word, ctx->str, ctx->token) == -1)
		return (free(word), -1);
	free(word);
	return (ctx->len);
}

static int	handle_unquoted_echo(t_echo_ctx *ctx, t_bau_args *meow)
{
	while (ctx->str[ctx->i + ctx->len]
		&& ctx->str[ctx->i + ctx->len] != '\''
		&& ctx->str[ctx->i + ctx->len] != '\"'
		&& ctx->str[ctx->i + ctx->len] != ' ')
	{
		if (ctx->str[ctx->i + ctx->len] == '$')
		{
			if (handle_env_var(ctx, meow) == -1)
				return (-1);
		}
		else
		{
			if (handle_plain_text(ctx) == -1)
				return (-1);
		}
	}
	return (ctx->len);
}

static t_echo_ctx	init_echo_context( t_token *token, char *str,
		t_bau_args *meow_bau, int i)
{
	t_echo_ctx	ctx;

	ctx.str = str;
	ctx.i = i;
	ctx.token = token;
	ctx.env = meow_bau->meow;
	ctx.envp = meow_bau->bau_bau;
	ctx.flag = false;
	ctx.len = skip_whitespaces(&ctx.str[ctx.i], NULL);
	ctx.output = ft_strdup("");
	return (ctx);
}

static void	echo_here_doc(t_echo_ctx *ctx, t_bau_args *meow)
{
	ctx->len += here_doc_init(ctx->token, ctx->str, ctx->i + ctx->len + 2) + 2;
	(void)meow;
}

static int	handle_echo_content(t_echo_ctx *ctx, t_bau_args *meow)
{
	char	c;

	if (ctx->str[ctx->i + ctx->len] == '-'
		&& ctx->str[ctx->i + ctx->len + 1] == 'n'
		&& (ctx->str[ctx->i + ctx->len + 2] == ' '
			|| ctx->str[ctx->i + ctx->len + 2] == '\0'))
	{
		ctx->flag = true;
		ctx->len += 2;
		while (ctx->str[ctx->i + ctx->len] == ' ')
			ctx->len++;
	}
	while (ctx->str[ctx->i + ctx->len]
		&& ctx->str[ctx->i + ctx->len] != ' ')
	{
		c = ctx->str[ctx->i + ctx->len];
		if (c == '\'')
		{
			if (handle_single_quote(ctx) == -1)
				return (-1);
		}
		else if (c == '\"')
		{
			if (handle_double_quote(ctx, meow) == -1)
				return (-1);
		}
		else if (c == '<' && ctx->str[ctx->i + ctx->len + 1] != '<')
			ctx->len += input_from_file(ctx->token,
					ctx->str, ctx->i + ctx->len + 1) + 1;
		else if (c == '<' && ctx->str[ctx->i + ctx->len + 1] == '<')
			echo_here_doc(ctx, meow);
		else if (c == '$')
		{
			if (handle_env_var(ctx, meow) == -1)
				return (-1);
		}
		else
		{
			if (handle_unquoted_echo(ctx, meow) == -1)
				return (-1);
		}
	}
	return (ctx->len);
}

int	ft_echo(t_token *token, char *str, int i, t_bau_args *meow_bau)
{
	t_echo_ctx	ctx;

	i += skip_whitespaces(&str[i], NULL);
	ctx = init_echo_context(token, str, meow_bau, i);
	if (!ctx.output)
		return (free_tokens_line(ctx.str, ctx.token, "malloc error"), -1);
	if (handle_echo_content(&ctx, meow_bau) == -1
		|| build_echo_arguments(ctx.token, ctx.output, ctx.flag, ctx.str) == -1)
	{
		free(ctx.output);
		return (-1);
	}
	free(ctx.output);
	return (ctx.len);
}
