/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 00:02:12 by chrlomba          #+#    #+#             */
/*   Updated: 2024/09/09 02:40:26 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

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

char	*extract_until_not_alfanum(char *str)
{
	int		i;
	int		k;
	char	*word;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == ' '))
		i++;
	word = ft_substr(str, 0, i);
	if (!word)
		return (NULL);
	i = 0;
	k = 0;
	while (word[i])
	{
		if (word[i] == ' ')
		{
			word[k++] = ' ';
			while (word[i] == ' ')
				i++;
		}
		else
			word[k++] = word[i++];
	}
	word[k] = '\0';
	return (word);
}

int	ft_echo(t_token *token, char *str, int i)
{
	int		len;
	bool	flag;
	char	quote;
	char	*word;

	len = skip_whitespaces(&str[i], NULL);
	flag = false;
	if (str[i + len] == '-' && str[i + len + 1] == 'n')
	{
		flag = true;
		len += 2;
	}
	len += skip_whitespaces(&str[i + len], NULL);
	quote = str[i + len];
	if (quote == '\'' || quote == '\"')
	{
		word = extract_word(&str[i + len + 1], quote);
		len += 2;
	}
	else
		word = extract_until_not_alfanum(&str[i + len]);
	if (!word)
		return (free_tokens_line(str, token, "malloc error"), -1);
	len += ft_strlen(word);
	ft_putstr_fd(word, 1);
	if (!flag)
		ft_putchar_fd('\n', 1);
	free(word);
	return (len);
}

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
	if (path[0] == '~')
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


int ft_pwd(void) {
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd");
        return -1;
    }
    ft_putendl_fd(cwd, 1);

    return 0;
}

int	ft_export(char **env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		ft_putendl_fd(env[i], 1);
		i++;
	}
	return (0);
}

int	ft_unset(t_token *token, char *str, int i)
{
	int		len;
	char	*name;

	len = skip_whitespaces(&str[i], NULL);
	name = extract_token(&str[i + len]);
	if (!name)
		return (free_tokens_line(str, token, "malloc error"), -1);
	len += ft_strlen(name);
	if (unsetenv(name) == -1)
	{
		free(name);
		return (free_tokens_line(str, token, "unset error"), -1);
	}
	free(name);
	return (len);
}

int	ft_env(char **env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		ft_putendl_fd(env[i], 1);
		i++;
	}
	return (0);
}

int	ft_exit(t_token *token, char *str)
{
	free_token(token);
	free(str);
	exit(0);
}

int	process_builtin(t_token *token, char *str, int i, t_state *state, char **env)
{
	*state = SKIP_WHITESPACE;
	(void)env;
	if (!ft_strcmp(token->token, "echo"))
		return (ft_echo(token, str, i));
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
	return (0);
}
