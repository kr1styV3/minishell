/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:09:12 by chrlomba          #+#    #+#             */
/*   Updated: 2025/01/05 13:55:11 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"
#include "../../headers/parsing.h"

static char **ft_unsetenv(char *name, char **env)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	while (env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * i);
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, ft_strlen(name)))
		{
			new_env[j] = ft_strdup(env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	ft_free_mtx(env);
	return (new_env);
}

int	ft_unset(t_token *token, char *str, int i, char **env)
{
	int		len;
	char	*name;

	len = skip_whitespaces(&str[i], NULL);
	name = extract_token(&str[i + len]);
	if (!name)
		return (free_tokens_line(str, token, "malloc error"), -1);
	len += ft_strlen(name);
	env = ft_unsetenv(name, env);
	if (!env)
		return (free_tokens_line(str, token, "env realloc error"), -1);
	free(name);
	return (len);
}
