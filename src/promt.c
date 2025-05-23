/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   promt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:18:43 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 20:18:47 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/builtins.h"
#include "env_variables.h"
#include "../my_libft/headers/my_va_list.h"

#include "../my_libft/headers/my_va_list.h"

#define GREEN       "\033[0;32m"
#define PURPLE      "\033[0;35m"
#define YELLOW      "\033[0;33m"
#define PINK        "\033[0;35m"
#define CYAN        "\033[0;36m"
#define RED         "\033[0;31m"
#define BOLD        "\033[1m"
#define BLUE		"\033[0;34m"
#define UNDERLINE   "\033[4m"
#define NC          "\033[0m"   // Reset color

void	get_system_hostname(char *hostname, size_t size)
{
	ssize_t		bytes_read;
	int			fd;

	fd = open("/proc/sys/kernel/hostname", O_RDONLY);
	if (fd == -1)
	{
		fd = open("/etc/hostname", O_RDONLY);
		if (fd == -1)
		{
			ft_strncpy(hostname, "unknown", size);
			return ;
		}
	}
	bytes_read = read(fd, hostname, size - 1);
	if (bytes_read > 0)
	{
		hostname[bytes_read] = '\0';
		hostname[ft_strcspn(hostname, "\n")] = '\0';
	}
	else
		ft_strncpy(hostname, "unknown", size);
	close(fd);
}

char	*replace_home_with_symbol(const char *pwd, const char *home)
{
	char		*new_pwd;
	const char	*symbol;
	size_t		pwd_len;
	size_t		home_len;

	symbol = "~";
	pwd_len = ft_strlen(pwd);
	home_len = ft_strlen(home);
	if (ft_strncmp(pwd, home, home_len) == 0)
	{
		new_pwd = (char *)malloc(ft_strlen(symbol) + pwd_len - home_len + 1);
		if (new_pwd == NULL)
			return (NULL);
		ft_strlcpy(new_pwd, symbol, ft_strlen(symbol) + 1);
		ft_strlcat(new_pwd, pwd + home_len,
			ft_strlen(symbol) + pwd_len - home_len + 1);
	}
	else
	{
		new_pwd = (char *)malloc(pwd_len + 1);
		if (new_pwd == NULL)
			return (NULL);
		ft_strlcpy(new_pwd, pwd, pwd_len + 1);
	}
	return (new_pwd);
}

char	*get_pwd_with_home_replacement(t_env_list *env)
{
	char		*pwd;
	char		*home;
	char		*shortened_pwd;

	pwd = ft_getenv("PWD", env);
	home = ft_getenv("HOME", env);
	shortened_pwd = replace_home_with_symbol(pwd, home);
	free(pwd);
	free(home);
	return (shortened_pwd);
}

char	*get_promt(t_env_list *env)
{
	char		*promt;
	char		*user;
	char		*pwd;
	char		host_name[256];
	t_join		join;

	user = ft_getenv("USER", env);
	pwd = get_pwd_with_home_replacement(env);
	get_system_hostname(host_name, 256);
	join.blue = BLUE;
	join.nc = NC;
	join.green = GREEN;
	join.user = user;
	join.cyan = CYAN;
	join.host_name = host_name;
	join.bold = BOLD;
	join.pwd = pwd;
	join.yellow = YELLOW;
	join.pink = PINK;
	promt = ft_multijoin(&join);
	free(user);
	free(pwd);
	return (promt);
}
