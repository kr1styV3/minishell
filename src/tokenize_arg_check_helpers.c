/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_check_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:08:14 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/04 19:29:04 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int	fill_unquoted_argument(t_token *token,
		const char *str, int pos, int arg_i)
{
	int		word_start;
	int		len;

	word_start = pos;
	len = 0;
	while (str[pos] && !ft_isspace(str[pos]) && !ft_strchr(OPERATORS, str[pos]))
	{
		pos++;
		len++;
	}
	token->arg[arg_i] = ft_substr(str, word_start, len);
	return (pos);
}

int	fill_quoted_argument(t_token *token,
		const char *str, int pos, int arg_i)
{
	char	quote;
	int		tmp;
	int		len;

	quote = str[pos];
	tmp = pos + 1;
	len = 0;
	while (str[tmp + len] && str[tmp + len] != quote)
		len++;
	token->arg[arg_i] = ft_substr(str, tmp, len);
	if (str[tmp + len] == quote)
		len++;
	return (tmp + len);
}

int	count_arguments(const char *str, int start)
{
	int	count;
	int	pos;
	int	len;

	count = 0;
	pos = start;
	while (str[pos])
	{
		pos += skip_whitespaces((char *)&str[pos], NULL);
		if (!str[pos] || ft_strchr(OPERATORS, str[pos]))
			break ;
		len = get_next_arg_len(str, &pos);
		if (len == 0)
			break ;
		count++;
	}
	return (count);
}
