/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:07:21 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/23 15:18:04 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

#define EXITSTDOUT "why \033[4;31mexit\033[0m my shell :("

static int	str_eql_pipe(char *str)
{
	int	found;
	int	i;

	found = 1;
	i = 0;
	while (str && str[i])
	{
		if (str[i] == '|')
			return (found);
		i++;
	}
	return (0);
}

int	ft_exit(char *str)
{
	if (str_eql_pipe(str) == 1)
		g_should_exit = 2;
	else
	{
		ft_putendl_fd(EXITSTDOUT, 1);
		g_should_exit = 1;
	}
	return (0);
}
