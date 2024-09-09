/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 04:54:29 by chrlomba          #+#    #+#             */
/*   Updated: 2024/09/09 05:01:45 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

// parsing.c
/**
 * @brief Reads a line from the user
 * @param token The token to be filled with the user input.
 * @return This function does not return; it writes directly to the token.
 */
void	read_line_from_user(t_token *token, char **env);
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
int process_token(t_token *token, char *str, int string_position, t_state *state);
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
int process_word(t_token *token, char *str, int string_position, t_state *state);
/**
 * @brief Processes an operator character from the input string.
 *
 * This function processes operator characters such as |, <, >, and stores them
 * in the provided `t_token` structure.
 *
 * @param token A pointer to the `t_token` structure where the operator will be stored.
 * @param str The input string containing the operator.
 * @param state The state of the parser to change to SKIP_WHITESPACE.
 * @return The number of characters processed (e.g., 2 for ">>" or 1 for other operators).
 */
int process_operator(t_token *token, char *str, int string_position, t_state *state);

/**
 * @brief Parses the string and fills the token structure.
 * @param str The string to be parsed.
 * @param token The token to be filled.
 * @return This function does not return; it writes directly to the token.
 */
void	tokenizer(char *str, t_token *token, char **env);

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
int process_operator(t_token *token, char *str, int string_position, t_state *state);

#endif
