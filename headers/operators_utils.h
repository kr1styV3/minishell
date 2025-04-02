/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators_utils.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:32:20 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/01 13:36:55 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATORS_UTILS_H
# define OPERATORS_UTILS_H

inline bool	is_append_op(char *s, int i)
{
	return (s[i] == '>' && s[i + 1] == '>');
}

inline bool	is_overwrite_op(char *s, int i)
{
	return (s[i] == '>' && s[i + 1] != '>');
}

inline bool	is_heredoc_op(char *s, int i)
{
	return (s[i] == '<' && s[i + 1] == '<');
}

inline bool	is_input_op(char *s, int i)
{
	return (s[i] == '<' && s[i + 1] != '<');
}

inline bool	is_file_cmd(char *s, int i)
{
	return (s[i] == '.' && s[i + 1] == '/');
}

#endif
