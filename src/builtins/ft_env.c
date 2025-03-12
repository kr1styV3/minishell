/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:08:19 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/10 14:54:34 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"

int	ft_env(t_env_list *env)
{
	while (env)
	{
		ft_putendl_fd(env->value, 1);
		env = env->next;
	}
	return (0);
}
