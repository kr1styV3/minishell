/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:37:20 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/08 15:35:44 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_H
# define HANDLER_H

#include "parsing.h"
#include "state.h"

/**
 * @brief Function pointer type for state handler functions.
 *
 * This type is used for functions that handle different states of the parser.
 * The function takes a token, the input string, the current position in the string,
 * a pointer to the current state, and the environment.
 *
 * @param token The current token being processed.
 * @param str The input string.
 * @param pos The current position in the string.
 * @param state Pointer to the current state of the parser.
 * @param env Pointer to the environment variables.
 * @return The number of characters processed (to increment the string position).
 */
typedef int (*state_handler_func)(t_token *token, char *str, int pos, t_state *state, char **env);


/**
 * @brief Array of state handler functions.
 *
 * Maps each parser state to its corresponding handler function. The handler
 * functions process tokens based on the current state.
 */

extern state_handler_func state_handlers[];


/**
 * @brief Handles the NORMAL state, processing alphanumeric tokens or transitioning to other states.
 *
 * @param token Pointer to the current token being processed.
 * @param str The input string being parsed.
 * @param pos The current position in the input string.
 * @param state Pointer to the current state of the parser.
 * @param env The environment variables.
 * @return The number of characters to move forward in the string.
 */
int handle_normal(t_token *token, char *str, int pos, t_state *state, char **env);

/**
 * @brief Handles the SKIP_WHITESPACE state by skipping over whitespace characters.
 *
 * @param token Pointer to the current token being processed.
 * @param str The input string being parsed.
 * @param pos The current position in the input string.
 * @param state Pointer to the current state of the parser.
 * @param env The environment variables.
 * @return The number of characters to move forward in the string.
 */
int handle_skip_whitespace(t_token *token, char *str, int pos, t_state *state, char **env);

/**
 * @brief Handles the IN_BUILTIN state by processing built-in commands.
 *
 * @param token Pointer to the current token being processed.
 * @param str The input string being parsed.
 * @param pos The current position in the input string.
 * @param state Pointer to the current state of the parser.
 * @param env The environment variables.
 * @return The number of characters to move forward in the string.
 */
int handle_in_builtin(t_token *token, char *str, int pos, t_state *state, char **env);

/**
 * @brief Handles the IN_VARIABLE state by processing environment variable expansions.
 *
 * @param token Pointer to the current token being processed.
 * @param str The input string being parsed.
 * @param pos The current position in the input string.
 * @param state Pointer to the current state of the parser.
 * @param env The environment variables.
 * @return The number of characters to move forward in the string.
 */
int handle_in_variable(t_token *token, char *str, int pos, t_state *state, char **env);

/**
 * @brief Handles the IN_WORD state by processing quoted strings.
 *
 * @param token Pointer to the current token being processed.
 * @param str The input string being parsed.
 * @param pos The current position in the input string.
 * @param state Pointer to the current state of the parser.
 * @param env The environment variables.
 * @return The number of characters to move forward in the string.
 */
int handle_in_word(t_token *token, char *str, int pos, t_state *state, char **env);

/**
 * @brief Handles the IN_OPERATOR state by processing operators like |, <, >.
 *
 * @param token Pointer to the current token being processed.
 * @param str The input string being parsed.
 * @param pos The current position in the input string.
 * @param state Pointer to the current state of the parser.
 * @param env The environment variables.
 * @return The number of characters to move forward in the string.
 */
int handle_in_operator(t_token *token, char *str, int pos, t_state *state, char **env);



# endif
