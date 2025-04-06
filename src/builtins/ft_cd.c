/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:12:05 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/04 20:04:37 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"
#include "../../headers/parsing.h"
#include "../../headers/env_variables.h"

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


int	ft_cd(t_token *token, char *str, int i, t_env_list *env)
{
	int		len;
	char	*path;

	len = skip_whitespaces(&str[i], NULL);
	path = extract_path(&str[i + len]);
	if (!path)
		return (free_tokens_line(str, token, "malloc error"), -1);
	if (path[0] == '~' || path[0] == '\0')
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
	return (len);
}
