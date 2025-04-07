/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:07:21 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 20:10:26 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

int	ft_exit(t_token *token, char *str)
{
	(void)token;
	(void)str;
	g_should_exit = 1;
	return (0);
}
