/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:06:00 by chrlomba          #+#    #+#             */
/*   Updated: 2024/11/27 19:06:07 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/builtins.h"
int process_builtin(t_token *token, char *str, int i, t_state *state, char **env)
{
    *state = FREE_TOKEN;

    if (!ft_strncmp(token->token, "echo", 4))
        return (ft_echo(token, str, i, env));
    if (!ft_strncmp(token->token, "cd", 2))
        return (ft_cd(token, str, i, env));
    if (!ft_strncmp(token->token, "pwd", 3))
        return (ft_pwd());
    if (!ft_strncmp(token->token, "export", 6))
        return (ft_export(env));
    if (!ft_strncmp(token->token, "unset", 5))
        return (ft_unset(token, str, i));
    if (!ft_strncmp(token->token, "env", 3))
        return (ft_env(env));
    if (!ft_strncmp(token->token, "exit", 4))
        return (ft_exit(token, str));

    free(token->token);
    token->token = NULL;
    return (0);
}

int ft_isbuiltin(char *cmd)
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
