/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:19:26 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/04 19:22:14 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_operator_block(char *line, int *i, int len)
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

int	check_syntax(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(line);
	while (i < len)
	{
		if (ft_isalnum(line[i]) || line[i] == '_'
			|| line[i] == '-' || line[i] == ','
			|| ft_isspace(line[i]))
		{
			i++;
			continue ;
		}
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
