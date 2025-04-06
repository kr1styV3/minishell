/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:08:19 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 19:06:52 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"

int	ft_env(t_token *token, t_env_list *env)
{
	char    *line;

	line = ft_strdup(env->value);
    if (!line)
        return (free_tokens_line(NULL, token, "malloc error for internal process"), -1);
    env=env->next;
    while (env)
    {
        line = ft_freejoin(line, "\n");
        if (!line)
            return (free_tokens_line(NULL, token, "malloc error for internal process"), -1);
        line = ft_freejoin(line, env->value);
        if (!line)
            return (free_tokens_line(NULL, token, "malloc error for internal process"), -1);
        env = env->next;
    }
    line = ft_freejoin(line, "\n");
    if (!line)
        return (free_tokens_line(NULL, token, "malloc error for internal process"), -1);
    token->arg = (char **)ft_calloc(3, sizeof(char *));
    token->arg[0] = ft_strdup("env");
    if (!token->arg[0])
        return (free_tokens_line(NULL, token, "malloc error for internal process"), -1);
    token->arg[1] = ft_strdup(line);
    if (!token->arg[1])
        return (free_tokens_line(NULL, token, "malloc error for internal process"), -1);
    token->arg[2] = NULL;
	token->checker = false;
    free(line);
	return (0);
}
