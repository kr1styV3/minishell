/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_check_syntax.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:19:26 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/11 13:58:45 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>
#include <string.h>

static int	check_operator_block(const char *line, int *i, int len)
{
	char	op;
	int		count;

	op = line[*i];
	count = 0;
	while (*i < len && line[*i] == op)
	{
		count++;
		(*i)++;
	}
	if (op == '|' && count > 1)
		return (1);
	if ((op == '<' || op == '>') && count > 2)
		return (1);
	if (*i < len && (line[*i] == '<' || line[*i] == '>' || line[*i] == '|'))
		return (1);
	return (0);
}

static int	check_quotes(const char *line, int len)
{
	int		i;
	char	q;
	int		closed;

	i = 0;
	while (i < len)
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			q = line[i++];
			closed = 0;
			while (i < len)
			{
				if (line[i] == q)
				{
					closed = 1;
					return (0);
				}
				i++;
			}
			if (!closed)
				return (1);
		}
		else
			i++;
	}
	return (0);
}

int	check_syntax(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = strlen(line);
	if (check_quotes(line, len))
		return (1);
	while (i < len)
	{
		if (line[i] == '<' || line[i] == '>' || line[i] == '|')
		{
			if (check_operator_block(line, &i, len))
				return (1);
		}
		else
			i++;
	}
	return (0);
}
