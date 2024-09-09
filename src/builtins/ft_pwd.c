/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:10:26 by chrlomba          #+#    #+#             */
/*   Updated: 2024/09/09 05:11:12 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"

int ft_pwd(void)
{
	char cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return -1;
	}
	ft_putendl_fd(cwd, 1);

	return (0);
}
