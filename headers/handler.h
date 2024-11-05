/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:37:20 by chrlomba          #+#    #+#             */
/*   Updated: 2024/11/05 13:31:19 by chrlomba         ###   ########.fr       */
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
 * @brief Maps parser states to their corresponding handler functions.
 *
 * This struct is used to associate a state in the parsing process (represented
 * by the `t_state` enum) with a handler function. The handler function processes
 * the input string based on the current state.
 *
 * @struct s_state_handler_map
 *
 * @var t_state_handler_map::state
 * The current state of the parser, represented by a value from the `t_state` enum.
 *
 * @var t_state_handler_map::handler
 * A function pointer of type `state_handler_func` that processes the input string
 * for the given state. The function takes a token, the input string, the current
 * position in the string, a pointer to the current state, and the environment.
 */
typedef struct s_state_handler_map
{
    t_state state;
    state_handler_func handler;
} t_state_handler_map;

/**
 * @brief Initializes the state-handler map.
 *
 * This function allocates memory for an array of `t_state_handler_map` structs and maps
 * each state in the `t_state` enum to its corresponding handler function. It returns the
 * allocated and initialized map.
 *
 * @return A pointer to the dynamically allocated `t_state_handler_map` array.
 *         Returns `NULL` if memory allocation fails.
 *
 * The handler map includes mappings for the following states:
 * - SKIP_WHITESPACE: Calls `handle_skip_whitespace`.
 * - IN_BUILTIN: Calls `handle_in_builtin`.
 * - IN_WORD: Calls `handle_in_word`.
 * - IN_OPERATOR: Calls `handle_in_operator`.
 * - IN_VARIABLE: Calls `handle_in_variable`.
 * - NORMAL: Calls `handle_normal`.
 */
t_state_handler_map *init_handler(void);

/**
 * @brief Retrieves the handler function for a given state.
 *
 * This function searches through the `handler_map` for a state that matches the
 * given `state` parameter. If a matching state is found, the corresponding handler
 * function is returned.
 *
 * @param handler_map A pointer to the array of `t_state_handler_map` structs that map states to handlers.
 * @param state The current state for which the corresponding handler is required.
 *
 * @return A function pointer (`state_handler_func`) to the handler function for the specified state.
 *         Returns `NULL` if no handler is found for the state.
 */
state_handler_func get_handler(t_state_handler_map *handler_map, t_state state);

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
