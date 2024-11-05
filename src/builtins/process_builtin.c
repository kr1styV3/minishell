/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:06:00 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/08 16:08:59 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/builtins.h"

int	process_builtin(t_token *token, char *str, int i, t_state *state, char **env)
{
	*state = SKIP_WHITESPACE;
	if (!ft_strcmp(token->token, "echo"))
		return (ft_echo(token, str, i, env));
	if (!ft_strcmp(token->token, "cd"))
		return (ft_cd(token, str, i, env));
	if (!ft_strcmp(token->token, "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(token->token, "export"))
		return (ft_export(env));
	if (!ft_strcmp(token->token, "unset"))
		return (ft_unset(token, str, i));
	if (!ft_strcmp(token->token, "env"))
		return (ft_env(env));
	if (!ft_strcmp(token->token, "exit"))
		return (ft_exit(token, str));
	free(token->token);
	token->token = NULL;
	return (0);
}

int	ft_isbuiltin(char *cmd)
{
	if(cmd)
	{
		if (!ft_strcmp(cmd, "echo"))
			return (1);
		if (!ft_strcmp(cmd, "cd"))
			return (1);
		if (!ft_strcmp(cmd, "pwd"))
			return (1);
		if (!ft_strcmp(cmd, "export"))
			return (1);
		if (!ft_strcmp(cmd, "unset"))
			return (1);
		if (!ft_strcmp(cmd, "env"))
			return (1);
		if (!ft_strcmp(cmd, "exit"))
			return (1);
	}
	return (0);
}
