/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:52:40 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/08 14:53:34 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_LINE_H
# define READ_LINE_H

# include "minishell.h"


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
void	read_line_from_user(t_token *token, char **env);

#endif