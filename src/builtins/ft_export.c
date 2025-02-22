/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:09:58 by chrlomba          #+#    #+#             */
/*   Updated: 2025/01/07 12:59:47 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"

static int	check_var(char *str, char **env)
{
	int	i;

	i = 0;
	if (!str)
		return (-2);
	if (!env)
		return (-2);
	while (env[i])
	{
		if (ft_strncmp(str, env[i], ft_strlen(str)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static int	export_cleanup(char *var_n, char *var_v, char *err)
{
	free(var_n);
	free(var_v);
	write(2, err, ft_strlen(err));
	return (-1);
}

static char	**env_realloc(char **env, char *var_n, char *var_v)
{
	char	**new_env;
	char	*value;
	char	*columns;
	int		i;

	value = ft_strjoin(var_n, var_v);
	i = 0;
	while (env[i])
		i++;
	new_env = (char **)ft_calloc(i + 2, sizeof(char *));
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (ft_strncmp("COLUMNS=", env[i], ft_strlen("COLUMNS=")))
		{
			columns = ft_strdup(env[i]);
			if (columns[ft_strlen(columns)] == '9')
			{
				columns[ft_strlen(columns)] = '0';
				columns[ft_strlen(columns) - 1] += 1;
			}
			else
				columns[ft_strlen(columns)] += 1;
			new_env[i] = ft_strdup(columns);
			free(columns);
		}
		i++;
	}
	ft_free_mtx(env);
	new_env[i] = ft_strdup(value);
	free(value);
	return (new_env);
}

static void	env_var(char **env, int idx, char *str1, char *str2)
{
	char	*value;

	value = ft_strjoin(str1, str2);
	free(env[idx]);
	env[idx] = ft_strdup(value);
	free(value);
}

int	ft_export(t_token *token, char **env, char *str, int _i)
{
	int		i;
	int		len;
	char	*var_name;
	char	*var_value;

	i = 0;
	len = skip_whitespaces(&str[_i], NULL);
	if (ft_isalnum(str[_i + len]))
	{
		var_name = extract_token(&str[_i + len]);
		len += ft_strlen(var_name);
		if (str[_i + len] == '=')
		{
			var_name = ft_freejoin(var_name, "=");
			_i += len + 1;
			var_value = extract_token(&str[_i]);
			i = check_var(var_name, env);
			if (i == -2)
				return (export_cleanup(var_name, var_value, "invalid env pointer"));
			else if (i == -1)
				env = env_realloc(env, var_name, var_value);
			else
				env_var(env, i, var_name, var_value);
			if (env == NULL)
				return (export_cleanup(var_name, var_value, "failed to reallocate space for env"));
			else
			{
				token->env_ptr = (void *)env;
				token->env_work = true;
			}
			len += ft_strlen(var_value) + 1;
			free(var_value);
		}
		// else
		// 	empty_var, so just do var=''
		free(var_name);
	}
	else
	{
		while (env[i])
		{
			ft_putendl_fd(env[i], 1);
			i++;
		}
	}
	return (len);
}
