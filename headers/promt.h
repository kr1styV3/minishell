/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   promt.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:28:23 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/17 20:42:32 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMT_H
# define PROMT_H

# include "../headers/minishell.h"
# include "../headers/builtins.h"
# include "../my_libft/headers/my_va_list.h"
# define GREEN       "\033[0;32m"
# define PURPLE      "\033[0;35m"
# define YELLOW      "\033[0;33m"
# define PINK        "\033[0;35m"
# define CYAN        "\033[0;36m"
# define RED         "\033[0;31m"
# define BOLD        "\033[1m"
# define BLUE		"\033[0;34m"
# define UNDERLINE   "\033[4m"
# define NC          "\033[0m"   // Reset color


/**
 * @brief Retrieves the system hostname and stores it in the provided buffer.
 *
 * This function attempts to read the hostname from `/proc/sys/kernel/hostname` (on Linux systems).
 * If that fails, it tries `/etc/hostname`. If both attempts fail, it sets the hostname to "unknown".
 *
 * @param hostname A pointer to the buffer where the hostname will be stored.
 * @param size The size of the buffer provided to store the hostname.
 */
void get_system_hostname(char *hostname, size_t size);

/**
 * @brief Replaces the home directory path in the PWD with a tilde (~).
 *
 * This function checks if the `pwd` starts with the `home` path. If it does, the home directory
 * is replaced with the tilde (`~`). Otherwise, it returns a copy of `pwd`.
 *
 * @param pwd The current working directory path.
 * @param home The user's home directory path.
 *
 * @return A newly allocated string where the home directory is replaced by `~`. The caller is responsible
 *         for freeing the returned string.
 */
char *replace_home_with_symbol(const char *pwd, const char *home);

/**
 * @brief Retrieves the current working directory (PWD) with the home directory replaced by `~`.
 *
 * This function fetches the `PWD` and `HOME` environment variables, then replaces the home directory in
 * `PWD` with a tilde (`~`), if applicable.
 *
 * @param env The environment variables array.
 *
 * @return A newly allocated string containing the current working directory with the home directory replaced
 *         by `~`. The caller is responsible for freeing the returned string.
 */
char *get_pwd_with_home_replacement(char **env);

/**
 * @brief Builds the shell prompt using various components like user, hostname, and PWD.
 *
 * This function creates a formatted shell prompt by fetching the user, hostname, and working directory
 * (with the home directory replaced by `~`). The components are then joined together with colors and formatting.
 *
 * @param env The environment variables array.
 *
 * @return A newly allocated string containing the shell prompt. The caller is responsible for freeing the returned string.
 */
char *get_promt(t_env_list *env);

# endif
