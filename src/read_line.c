/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/03/20 15:28:42 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parsing.h"
#include "../headers/env_variables.h"
#include "promt.h"
#include "read_line.h"
#include "t_token.h"

#define DELIMITERS " \t\r\n\a"

#define OPERATORS "<>|"
#define OPERATORSS "<>|"

#define CHECKWORDARG " "

#include "minishell.h"

/*
** ft_checkwordarg:
** Looks for a word or a quoted string starting at position `i` in `str`.
** If found, it stores that argument in (*token)->arg[1] and returns the
** number of characters consumed. If there is no argument, returns 0.
**
** Pseudocode:
**   1) Skip leading whitespaces.
**   2) If at an operator or end of string, no argument -> return 0.
**   3) If next char is a quote, read until matching quote.
**   4) Otherwise, read until whitespace, operator, or end-of-string.
**   5) Store the substring in (*token)->arg[1].
**   6) Skip trailing whitespace (optional).
**   7) Return total length consumed.
*/

/*
** get_next_arg_len():
**   - Skips leading whitespace (optional).
**   - If the next token is quoted, scans until the closing quote.
**   - Else scans until whitespace, operator, or end-of-string.
**   - Returns the length of the argument + updates `pos` to point
**     right after that argument (including its closing quote if any).
**
**   NOTE: We do NOT stop or break if we see whitespace in the middle
**   of a quoted string. Quoted sequences can have spaces inside them.
*/
static int get_next_arg_len(const char *str, int *pos)
{
    int  i = *pos;
    int  len = 0;
    char quote;

    // If already at '\0' or operator, no argument to measure
    if (!str[i] || ft_strchr(OPERATORS, str[i]))
        return 0;

    // Quoted argument?
    if (str[i] == '\'' || str[i] == '\"')
    {
        quote = str[i];
        i++;  // skip opening quote
        while (str[i + len] && str[i + len] != quote)
            len++;
        // If we find the matching quote, move past it
        if (str[i + len] == quote)
            len++;  // Include closing quote in the total length
    }
    else
    {
        // Unquoted: read until whitespace, operator, or end
        while (str[i + len] &&
               !ft_isspace(str[i + len]) &&
               !ft_strchr(OPERATORS, str[i + len]))
        {
            len++;
        }
    }

    // Update *pos to point right after this argument
    *pos = i + len;
    return len;
}


static int count_arguments(const char *str, int start)
{
    int count = 0;
    int pos   = start;
    int len;

    while (str[pos])
    {
        // Skip leading whitespace before the next argument
        pos += skip_whitespaces((char *)&str[pos], NULL);

        // If we're at end or at an operator, stop
        if (!str[pos] || ft_strchr(OPERATORS, str[pos]))
            break;

        // Get length of next argument
        len = get_next_arg_len(str, &pos);
        if (len == 0)
            break; // Possibly error or just no more arguments

        count++;
        // We do NOT stop on whitespace; we loop again, which will skip
        // any trailing whitespace at the top of the loop.
    }
    return count;
}


static void fill_arguments(t_token **token, const char *str, int start, int count)
{
    int arg_i = 1; // We'll put arguments in arg[1..count]
    int pos   = start;
    int len;

    while (str[pos] && arg_i <= count)
    {
        // Skip leading whitespace
        pos += skip_whitespaces((char *)&str[pos], NULL);

        // If we reached operator or end, stop
        if (!str[pos] || ft_strchr(OPERATORS, str[pos]))
            break;

        // Check if next is quoted
        if (str[pos] == '\'' || str[pos] == '\"')
        {
            char quote = str[pos];
            int  tmp   = pos + 1; // substring start (inside quotes)
            // measure length ignoring the opening quote
            len = 0;
            while (str[tmp + len] && str[tmp + len] != quote)
                len++;
            // store the substring inside quotes
            (*token)->arg[arg_i] = ft_substr(str, tmp, len);

            // skip the entire quoted area (including closing quote if found)
            // if we found the closing quote, skip it
            if (str[tmp + len] == quote)
                len++;
            pos = tmp + len;
        }
        else
        {
            // Unquoted
            int word_start = pos;
            len = 0;
            while (str[pos] &&
                   !ft_isspace(str[pos]) &&
                   !ft_strchr(OPERATORS, str[pos]))
            {
                pos++;
                len++;
            }
            (*token)->arg[arg_i] = ft_substr(str, word_start, len);
        }
        arg_i++;
    }
}


int	ft_checkwordarg(t_token **token, char *str, int i)
{
    int  start_pos = i;

    // If the token is a builtin, skip parsing further arguments
    if (ft_isbuiltin((*token)->parsed->token))
        return 0;

    // 1) Skip any whitespace before we start counting
    i += skip_whitespaces(&str[i], NULL);

    // 2) Count how many arguments are present from 'i' until an operator or end
    int arg_count = count_arguments(str, i);

    // If there are no arguments, return 0 or just how many we skipped
    if (arg_count == 0)
    {
        (*token)->arg = (char **)ft_calloc(2, sizeof(char *));
        return (i - start_pos);
    }

    // 3) Allocate arg array: +2 means index 0 (reserved) + arg_count + NULL
    //    e.g. if arg_count = 3, we want indices [0..3], plus arg[4] = NULL
    (*token)->arg = ft_calloc(arg_count + 2, sizeof(char *));
    // We'll fill [0] later with the correct path
    // For now, set it NULL:
    (*token)->arg[0] = NULL;

    // 4) Fill arguments into (*token)->arg[1..arg_count]
    fill_arguments(token, str, i, arg_count);

    // 5) After filling, find out how far we actually moved.
    //    We'll do a quick loop to move 'i' forward the same way fill_arguments did.
    //    Or we can replicate the logic from fill_arguments. For simplicity, do:
    int arg_i = 1;
    while (arg_i <= arg_count && (*token)->arg[arg_i])
        arg_i++;
    // 'arg_i - 1' is the actual number of arguments we found & stored.

    // Let's re-scan to know the final 'i' position:
    // (Alternatively, we can rewrite fill_arguments to return the final position.)
    {
        int fill_pos = i;
        int consumed_args = 0;
        while (str[fill_pos] && consumed_args < arg_count)
        {
            fill_pos += skip_whitespaces(&str[fill_pos], NULL);
            if (!str[fill_pos] || ft_strchr(OPERATORS, str[fill_pos]))
                break;
            // just skip next arg length
            get_next_arg_len(str, &fill_pos);
            consumed_args++;
        }
        // Now fill_pos is right after the last argument
        i = fill_pos;
    }

    // Return how many total characters we consumed from the start
    return (i - start_pos);
}




void	tokenizer(char *str, t_token *token, t_env_list *env)
{
	int string_position;
	t_token *next;
	t_state state;

	string_position = 0;
	state = SKIP_WHITESPACE;
	while (str[string_position])
	{
		if (state == SKIP_WHITESPACE)
			string_position += skip_whitespaces(&str[string_position], &state);
		if (ft_strchr(OPERATORS, str[string_position]))
		{
			string_position += process_operator(&token, str, string_position, &state);
			string_position += skip_whitespaces(&str[string_position], &state);
		}
		if (state == NORMAL)
		{
			if (ft_isalnum(str[string_position]))
				string_position += process_token(&token, str, string_position, &state);
			string_position += ft_checkwordarg(&token, str, string_position);
            string_position += skip_whitespaces(&str[string_position], &state);
			printf("%p\n", token->parsed);
			if (check_var(&token, str, &string_position, &env) == 0)
			{
				token->env_work = true;
				token->exec = false;
			}
			if (str[string_position] == 34 || str[string_position] == 39)
            state = IN_WORD;
			if (ft_strchr(OPERATORS, str[string_position]))
            state = IN_OPERATOR;
			if (ft_isbuiltin(token->parsed->token))
				state = IN_BUILTIN;
			if (str[string_position] == '$' && !IN_BUILTIN)
				state = IN_VARIABLE;
		}
		if (state == IN_BUILTIN)
        {
			string_position += process_builtin(&token, str, string_position, &state, env);
            string_position += skip_whitespaces(&str[string_position], &state);
        }
		if (state == IN_VARIABLE)
			string_position += process_variable(&token, str, string_position + 1, env) + 1;
		if (state == IN_WORD)
			string_position += process_word(&token, str, string_position, &state, env);
        while (ft_strchr(OPERATORS, str[string_position]))
        {
            if (state == IN_OPERATOR || ft_strchr(OPERATORS, str[string_position]))
                string_position += process_operator(&token, str, string_position, &state);
            string_position += skip_whitespaces(&str[string_position], &state);
            if (ft_strchr(OPERATORS, str[string_position]))
                string_position += process_operator(&token, str, string_position, &state);
            string_position += skip_whitespaces(&str[string_position], &state);
        }
		if (str[string_position] == '\0' && state == FREE_TOKEN)
		{
			return ;
		}
		else if (str[string_position] == '\0')
		{
			return ;
		}
		else if (should_exit)
		{
			return ;
		}
		else
		{
			next = init_token();
			token->next = next;
			token = token->next;
			state = SKIP_WHITESPACE;
		}
	}
}

void read_line_from_user(t_token **token, t_env_list *env, char **envp)
{
	char	*read_line;
	char	*promt;

	read_line = NULL;
	(void)envp;
	promt = get_promt(env);
	read_line = readline(promt);
	if (should_exit > 0)
	{
		(*token)->exec = false;
		free(promt);
		free(read_line);
		return;
	}
	while ((!read_line && !should_exit) || !ft_strlen(read_line))
		read_line = readline(promt);
	if (should_exit > 0)
	{
		(*token)->exec = false;
		free(promt);
		free(read_line);
		return;
	}
	free(promt);
	if (!read_line || should_exit)
		return ;
	add_history(read_line);
	tokenizer(read_line, *token, env);
	free(read_line);
	read_line = NULL;
}
