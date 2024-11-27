/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:34:57 by chrlomba          #+#    #+#             */
/*   Updated: 2024/11/22 15:55:30 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
# define STATE_H

typedef enum
{
	NORMAL,         // Default state: reading alphanumeric tokens.
	IN_BUILTIN,     // Inside a builtin command.
	IN_WORD,        // Inside a quoted string.
	IN_OPERATOR,    // Reading an operator like |, <, >.
	IN_VARIABLE,    // Reading a variable name.
	SKIP_WHITESPACE, // Skipping spaces.
	FREE_TOKEN
}	t_state;

#endif
