/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:06:00 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/23 15:15:08 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/builtins.h"
#include "../headers/executor.h"

int	process_builtin(t_token **token, char *str, int i,
			t_bau_args *meow)
{
	if (!ft_strcmp((*token)->parsed->token, "echo"))
		return (ft_echo(*token, str, i, meow));
	if (!ft_strcmp((*token)->parsed->token, "cd"))
		return (ft_cd(*token, str, i, meow->meow));
	if (!ft_strcmp((*token)->parsed->token, "pwd"))
		return (ft_pwd(*token));
	if (!ft_strcmp((*token)->parsed->token, "export"))
		return (ft_export(*token, &meow->meow, str, i));
	if (!ft_strcmp((*token)->parsed->token, "unset"))
		return (ft_unset(*token, str, i, &meow->meow));
	if (!ft_strcmp((*token)->parsed->token, "env"))
		return (ft_env(*token, meow->meow));
	if (!ft_strcmp((*token)->parsed->token, "exit"))
		return (ft_exit(str));
	ft_putstr_fd("minishell : builtin not found : ", 2);
	ft_putendl_fd((*token)->parsed->token, 2);
	g_should_exit = 2;
	return (0);
}

int	ft_isbuiltin(char *cmd)
{
	if (cmd)
	{
		if (!ft_strncmp(cmd, "echo", 4))
			return (1);
		if (!ft_strncmp(cmd, "cd", 2))
			return (1);
		if (!ft_strncmp(cmd, "pwd", 3))
			return (1);
		if (!ft_strncmp(cmd, "export", 6))
			return (1);
		if (!ft_strncmp(cmd, "unset", 5))
			return (1);
		if (!ft_strncmp(cmd, "env", 3))
			return (1);
		if (!ft_strncmp(cmd, "exit", 4))
			return (1);
	}
	return (0);
}
