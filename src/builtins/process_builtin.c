/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:06:00 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/21 18:26:48 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/builtins.h"
int process_builtin(t_token **token, char *str, int i, t_state *state, t_env_list *env, char **envp)
{
    *state = FREE_TOKEN;

    if (!ft_strncmp((*token)->parsed->token, "echo", 4))
        return (ft_echo(*token, str, i, env, envp));
    if (!ft_strncmp((*token)->parsed->token, "cd", 2))
        return (ft_cd(*token, str, i, env));
    if (!ft_strncmp((*token)->parsed->token, "pwd", 3))
        return (ft_pwd(*token));
    if (!ft_strncmp((*token)->parsed->token, "export", 6))
        return (ft_export(*token, &env, str, i));
    if (!ft_strncmp((*token)->parsed->token, "unset", 5))
        return (ft_unset(*token, str, i, &env));
    if (!ft_strncmp((*token)->parsed->token, "env", 3))
        return (ft_env(*token, env));
    if (!ft_strncmp((*token)->parsed->token, "exit", 4))
        return (ft_exit(*token, str));
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
