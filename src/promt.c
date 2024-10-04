/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   promt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr> >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 13:39:35 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/04 16:20:52 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/builtins.h"
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

void get_system_hostname(char *hostname, size_t size)
{
    int fd = open("/proc/sys/kernel/hostname", O_RDONLY);
    if (fd == -1)
	{
        // If /proc/sys/kernel/hostname cannot be opened, try /etc/hostname
        fd = open("/etc/hostname", O_RDONLY);
        if (fd == -1)
		{
            // If both fail, set hostname to "unknown"
            ft_strncpy(hostname, "unknown", size);
            return;
        }
    }
    ssize_t bytes_read = read(fd, hostname, size - 1);
    if (bytes_read > 0)
	{
        hostname[bytes_read] = '\0';  // Null-terminate the string
        // Remove any trailing newline character
        hostname[ft_strcspn(hostname, "\n")] = '\0';
    }
	else
	{
        // If read fails, set hostname to "unknown"
        ft_strncpy(hostname, "unknown", size);
    }
    close(fd);
}

char *get_promt(char **env)
{
	char *promt;
	char *user;
	char *pwd;
	char host_name[256];

	user = ft_getenv("USER", env);
	pwd = ft_getenv("PWD", env);
	get_system_hostname(host_name, 256);
	promt = ft_multijoin(27, BLUE, "[", NC, GREEN, user, NC, "@", CYAN, host_name, NC, BLUE, "]", NC, " - ", BOLD, pwd, NC, BLUE, "]", NC, "\n", YELLOW, "Minishell", NC, PINK, " $ ", NC);
	free(user);
	free(pwd);
	return (promt);
}
