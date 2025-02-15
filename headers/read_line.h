/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coca <coca@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:52:40 by chrlomba          #+#    #+#             */
/*   Updated: 2025/02/15 09:57:00 by coca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_LINE_H
# define READ_LINE_H

# include "minishell.h"
# include "parsing.h"

/**
 * @brief Parses the string and fills the token structure.
 * @param str The string to be parsed.
 * @param token The token to be filled.
 * @return This function does not return; it writes directly to the token.
 */
void	tokenizer(char *str, t_token *token, char **env);

/**
 * @brief Reads a line from the user
 * @param token The token to be filled with the user input.
 * @return This function does not return; it writes directly to the token.
 */
void	read_line_from_user(t_token **token, t_env_list *env);

#endif
