/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 04:54:29 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/18 15:41:37 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# include "state.h"
# include "t_token.h"

// parsing.c

char *extract_file_token(char *str);
/**
 * @brief Skips the whitespaces in the given string.
 * @param str The string from which the whitespaces are skipped.
 * @param state The state of the parser to change to NORMAL state.
 * @return The number of characters skipped.
 */
int	skip_whitespaces(char *str, t_state *state);
/**
 * @brief Extracts an alphanumeric token from the string.
 *
 * This function reads from the beginning of the input string and extracts an
 * alphanumeric token, allocating memory for it.
 *
 * @param str The input string to extract the token from.
 * @return A pointer to the extracted token, or NULL if memory allocation fails.
 */
char *extract_token(char *str);
/**
 * @brief Extracts a word from a quoted string.
 *
 * This function extracts a word from a string enclosed by either single ('')
 * or double ("") quotes, stopping at the closing quote.
 *
 * @param str The input string starting after the opening quote.
 * @param quote The quote character used to enclose the word.
 * @return A pointer to the extracted word, or NULL if memory allocation fails.
 */
char *extract_word(char *str, char quote);
/**
 * @brief Processes an alphanumeric token from the input string.
 *
 * This function processes an alphanumeric token, extracting it from the input
 * string and storing it in the provided `t_token` structure.
 *
 * @param token A pointer to the `t_token` structure where the token will be stored.
 * @param str The input string to extract the token from.
 * @param state The state of the parser to change to SKIP_WHITESPACE.
 * @return The number of characters processed or -1 if memory allocation fails.
 */
int process_token(t_token **token, char *str, int string_position, t_state *state);
/**
 * @brief Processes a quoted word from the input string.
 *
 * This function processes a word enclosed in single or double quotes, extracting
 * it and storing it in the provided `t_token` structure.
 *
 * @param token A pointer to the `t_token` structure where the word will be stored.
 * @param str The input string starting with a quote.
 * @param state The state of the parser to change to SKIP_WHITESPACE.
 * @return The number of characters processed or -1 if memory allocation fails.
 */
int process_word(t_token **token, char *str, int string_position, t_state *state, t_env_list *env);

// operators.c
/**
 * @brief Check and set the file descriptor for append output redirection (>>).
 *
 * This function identifies the file for append output redirection, skips leading
 * whitespaces, extracts the file name, and opens the file with appropriate permissions.
 * If the file cannot be opened, the function handles the error.
 *
 * @param token Pointer to the token structure where file descriptor is stored.
 * @param str Input string containing the command.
 * @param string_position Position in the input string to start processing.
 * @return int The length of the processed string or -1 if an error occurs.
 */
int check_append_fd(t_token *token, char *str, int string_position);
/**
 * @brief Check and set the file descriptor for overwrite output redirection (>).
 *
 * This function handles the file for overwrite output redirection, skipping
 * whitespaces, extracting the file name, and opening the file with truncation mode.
 * If the file cannot be opened, it handles the error.
 *
 * @param token Pointer to the token structure where file descriptor is stored.
 * @param str Input string containing the command.
 * @param string_position Position in the input string to start processing.
 * @return int The length of the processed string or -1 if an error occurs.
 */
int check_overwrite_fd(t_token *token, char *str, int string_position);
/**
 * @brief Handle here-document input redirection (<<).
 *
 * This function reads input from the user line-by-line until the delimiter is encountered.
 * It concatenates the lines into a buffer that can be used as input for a command.
 *
 * @param token Pointer to the token structure where here-doc input is stored.
 * @param str Input string containing the command.
 * @param string_position Position in the input string to start processing.
 * @return int The length of the processed string or -1 if an error occurs.
 */
int here_doc(t_token *token, char *str, int string_position);
/**
 * @brief Check and set the file descriptor for input redirection (<).
 *
 * This function identifies the file for input redirection, skips leading
 * whitespaces, extracts the file name, and opens the file for reading.
 * If the file cannot be opened, the function handles the error.
 *
 * @param token Pointer to the token structure where file descriptor is stored.
 * @param str Input string containing the command.
 * @param string_position Position in the input string to start processing.
 * @return int The length of the processed string or -1 if an error occurs.
 */
int input_from_file(t_token *token, char *str, int string_position);
/**
 * @brief Process operators like pipes, redirections, and background execution.
 *
 * This function identifies and processes various shell operators, such as pipes (|),
 * append redirection (>>), overwrite redirection (>), here-doc (<<), input redirection (<),
 * and background execution (&), setting the appropriate flags and file descriptors.
 *
 * @param token Pointer to the token structure where operator information is stored.
 * @param str Input string containing the command.
 * @param string_position Position in the input string to start processing.
 * @param state Pointer to the state machine's current state.
 * @return int The length of the processed string or 0 if the operator doesn't modify the length.
 */
int process_operator(t_token **token, char *str, int string_position, t_state *state);

// flags.c


/**
 * @brief Extracts the length of the next flag in the string.
 *
 * This function iterates through the string starting from `start_pos`,
 * counting alphanumeric characters and hyphens ('-') to determine
 * the length of the next flag.
 *
 * @param str The input string containing flags.
 * @param start_pos The starting position in the string to begin extraction.
 *
 * @return The length of the next flag.
 */
int extract_flag_length(const char *str, int start_pos);

/**
 * @brief Allocates memory and copies a flag substring from the source string.
 *
 * This function allocates memory for a flag based on the specified `length`
 * and copies the substring from `str` starting at `start_pos` into the
 * newly allocated memory.
 *
 * @param str The input string containing flags.
 * @param start_pos The starting position in the string to copy from.
 * @param length The number of characters to copy for the flag.
 *
 * @return A pointer to the newly allocated and copied flag string.
 *         Returns `NULL` if memory allocation fails.
 */
char *allocate_and_copy_flag(const char *str, int start_pos, int length);

/**
 * @brief Handles memory allocation errors by freeing resources and reporting the error.
 *
 * This function is invoked when a memory allocation error occurs. It frees
 * the allocated tokens using `free_tokens_line` and returns an error code.
 *
 * @param str The input string being parsed.
 * @param token A pointer to the `t_token` structure containing allocated resources.
 * @param error_message A descriptive error message to be used for logging or reporting.
 *
 * @return Returns `-1` to indicate an error.
 */
int handle_allocation_error(char *str, t_token *token, char *error_message);

/**
 * @brief Assigns temporary flags to the `token->args` array.
 *
 * This function transfers the flags stored in the `tmp_flags` array into
 * the `args` member of the `t_token` structure. It starts assigning from
 * index 1 to accommodate any reserved positions (e.g., program name).
 *
 * @param token A pointer to the `t_token` structure where flags are to be assigned.
 * @param tmp_flags An array of temporary flag strings.
 * @param args_pos The number of flags parsed and stored in `tmp_flags`.
 *
 * @return Returns `0` on successful assignment.
 */
int assign_flags_to_token_args(t_token **token, char *tmp_flags[], int args_pos);

/**
 * @brief Parses flags from a string and assigns them to a token structure.
 *
 * The `parse_flags` function processes a segment of the input string starting
 * at `string_position`, extracting flags, handling memory allocation, and
 * assigning the parsed flags to the `args` member of the provided `t_token`.
 *
 * @param token A pointer to the `t_token` structure to which parsed flags will be assigned.
 * @param str The input string containing flags to parse.
 * @param string_position The starting position in the string to begin parsing.
 *
 * @return On success, returns the number of characters processed.
 *         On failure, returns `-1`.
 */
int parse_flags(t_token *token, char *str, int string_position);

#endif
