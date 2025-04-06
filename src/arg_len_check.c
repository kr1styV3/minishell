/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_len_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:14:56 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/04 19:15:48 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	arg_len_quoted(const char *str, int i, char quote)
{
	int	len;

	len = 0;
	while (str[i + len] && str[i + len] != quote)
		len++;
	if (str[i + len] == quote)
		len++;
	return (len);
}

static int	arg_len_unquoted(const char *str, int i)
{
	int	len;

	len = 0;
	while (str[i + len]
		&& !ft_isspace(str[i + len])
		&& !ft_strchr(OPERATORS, str[i + len]))
		len++;
	return (len);
}

int	get_next_arg_len(const char *str, int *pos)
{
	int		i;
	int		len;
	char	quote;

	i = *pos;
	len = 0;
	if (!str[i] || ft_strchr(OPERATORS, str[i]))
		return (0);
	if (str[i] == '\'' || str[i] == '\"')
	{
		quote = str[i];
		i++;
		len = arg_len_quoted(str, i, quote);
	}
	else
		len = arg_len_unquoted(str, i);
	*pos = i + len;
	return (len);
}
