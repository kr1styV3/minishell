/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:12:05 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/08 12:57:15 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"
#include "../../headers/parsing.h"

char	*extract_path(char *str)
{
	int		k;
	char	*path;

	k = 0;
	while (str[k] && str[k] != ' ')
		k++;
	path = ft_substr(str, 0, k);
	if (!path)
		return (NULL);
	return (path);
}

char	*ft_getenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] == name[j])
			j++;
		if (env[i][j] == '=' && !name[j])
		{
			value = ft_strdup(&env[i][j + 1]);
			if (!value)
				return (NULL);
			return (value);
		}
		i++;
	}
	return (NULL);
}

int	ft_cd(t_token *token, char *str, int i, char **env)
{
	int		len;
	char	*path;

	len = skip_whitespaces(&str[i], NULL);
	path = extract_path(&str[i + len]);
	if (!path)
		return (free_tokens_line(str, token, "malloc error"), -1);
	if (path[0] == '~' || !path[0])
	{
		free(path);
		path = ft_strdup(ft_getenv("HOME", env));
		if (!path)
			return (free_tokens_line(str, token, "malloc error"), -1);
	}
	len += ft_strlen(path);
	if (chdir(path) == -1)
	{
		free(path);
		return (free_tokens_line(str, token, "cd error"), -1);
	}
	free(path);
	free(token->token);
	return (len);
}
