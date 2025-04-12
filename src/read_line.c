/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:16:18 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/12 12:07:30 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../headers/env_variables.h"
#include "promt.h"
#include "read_line.h"
#include "t_token.h"

#define DELIMITERS " \t\r\n\a"

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
// */
// static int get_next_arg_len(const char *str, int *pos)
// {
//     int  i = *pos;
//     int  len = 0;
//     char quote;

//     // If already at '\0' or operator, no argument to measure
//     if (!str[i] || ft_strchr(OPERATORS, str[i]))
//         return 0;

//     // Quoted argument?
//     if (str[i] == '\'' || str[i] == '\"')
//     {
//         quote = str[i];
//         i++;  // skip opening quote
//         while (str[i + len] && str[i + len] != quote)
//             len++;
//         // If we find the matching quote, move past it
//         if (str[i + len] == quote)
//             len++;  // Include closing quote in the total length
//     }
//     else
//     {
//         // Unquoted: read until whitespace, operator, or end
//         while (str[i + len] &&
//                !ft_isspace(str[i + len]) &&
//                !ft_strchr(OPERATORS, str[i + len]))
//         {
//             len++;
//         }
//     }

//     // Update *pos to point right after this argument
//     *pos = i + len;
//     return len;
// }
// static int count_arguments(const char *str, int start)
// {
//     int count = 0;
//     int pos   = start;
//     int len;

//     while (str[pos])
//     {
//         // Skip leading whitespace before the next argument
//         pos += skip_whitespaces((char *)&str[pos], NULL);

//         // If we're at end or at an operator, stop
//         if (!str[pos] || ft_strchr(OPERATORS, str[pos]))
//             break;

//         // Get length of next argument
//         len = get_next_arg_len(str, &pos);
//         if (len == 0)
//             break; // Possibly error or just no more arguments

//         count++;
//         // We do NOT stop on whitespace; we loop again, which will skip
//         // any trailing whitespace at the top of the loop.
//     }
//     return count;
// }
// static void fill_arguments(t_token *token
// , const char *str, int start, int count)
// {
//     int arg_i = 1; // We'll put arguments in arg[1..count]
//     int pos   = start;
//     int len;

//     while (str[pos] && arg_i <= count)
//     {
//         // Skip leading whitespace
//         pos += skip_whitespaces((char *)&str[pos], NULL);

//         // If we reached operator or end, stop
//         if (!str[pos] || ft_strchr(OPERATORS, str[pos]))
//             break;

//         // Check if next is quoted
//         if (str[pos] == '\'' || str[pos] == '\"')
//         {
//             char quote = str[pos];
//             int  tmp   = pos + 1; // substring start (inside quotes)
//             // measure length ignoring the opening quote
//             len = 0;
//             while (str[tmp + len] && str[tmp + len] != quote)
//                 len++;
//             // store the substring inside quotes
//             token->arg[arg_i] = ft_substr(str, tmp, len);

//             // skip the entire quoted area (including closing quote if found)
//             // if we found the closing quote, skip it
//             if (str[tmp + len] == quote)
//                 len++;
//             pos = tmp + len;
//         }
//         else
//         {
//             // Unquoted
//             int word_start = pos;
//             len = 0;
//             while (str[pos] &&
//                    !ft_isspace(str[pos]) &&
//                    !ft_strchr(OPERATORS, str[pos]))
//             {
//                 pos++;
//                 len++;
//             }
//             token->arg[arg_i] = ft_substr(str, word_start, len);
//         }
//         arg_i++;
//     }
// }
// // void	tokenizer(char *str, t_token *token, t_env_list *env, char **envp)
// // {
// // 	int string_position;
// // 	t_token *next;
// // 	t_state state;

// // 	string_position = 0;
// // 	state = SKIP_WHITESPACE;
// // 	while (str[string_position])
// // 	{
// // 		if (state == SKIP_WHITESPACE)
// // 			string_position += skip_whitespac
// es(&str[string_position], &state);
// //         if (str[string_position] == '.'){
// //             string_position += process_ope
// rator(&token, str, string_position, &state);
// //             string_position += skip_whites
// paces(&str[string_position], &state);
// //         }
// // 		if (ft_strchr(OPERATORS, str[string_position]))
// // 		{
// // 			string_position += process_opera
// tor(&token, str, string_position, &state);
// // 			string_position += skip_whitesp
// aces(&str[string_position], &state);
// //             if (str[string_position] == '\0')
// //                 token->checker = false;
// // 		}
// // 		if (state == NORMAL)
// // 		{
// // 			if (ft_isalnum(str[string_position]))
// // 				string_position += process_t
// oken(&token, str, string_position, &state);
// //             if (check_var(&token, str, &string_position, &env) == 0)
// //             {
// //                 token->env_work = true;
// //                 token->exec = false;
// //             }
// // 			string_position += ft_checkwo
// rdarg(&token, str, string_position);
// //             string_position += skip_white
// spaces(&str[string_position], &state);
// //             if (!token->arg && token->chec
// ker == true && !ft_isbuiltin(token->parsed->token))
// //                 token->arg = (char **)ft_
// calloc(2, sizeof(char *));
// // 			if (str[string_position] == 34 || str[string_position] == 39)
// //             state = IN_WORD;
// // 			if (ft_strchr(OPERATORS, str[string_position]))
// //             state = IN_OPERATOR;
// // 			if (ft_isbuiltin(token->parsed->token))
// // 				state = IN_BUILTIN;
// // 			if (str[string_position] == '$' && !IN_BUILTIN)
// // 				state = IN_VARIABLE;
// // // 		}
// // // 		if (state == IN_BUILTIN)
// // //         {
// // // 			string_position += process_bui
// ltin(&token, str, string_position, &state, env, envp);
// // //             string_position += skip_whites
// paces(&str[string_position], &state);
// // //         }
// // // 		if (state == IN_VARIABLE)
// // // 			string_position += process_variabl
// e(&token, str, string_position + 1, env, envp) + 1;
// // // 		if (state == IN_WORD)
// // // 			string_position += process_wor
// d(&token, str, string_position, &state, env);
// // //         while (ft_strchr(OPERATORS, str[string_position]))
// // //         {
// // //             if (state == IN_OPERATOR || ft
// _strchr(OPERATORS, str[string_position]))
// // //                 string_position += proc
// ess_operator(&token, str, string_position, &state);
// // //             if (str[string_position] == '\0')
// // //                     return ;
// //             string_position += skip_whitespaces(&s
// tr[string_position], &state);
// //         }
// // 		if (str[string_position] == '\0' && state == FREE_TOKEN)
// // 		{
// // 			return ;
// // 		}
// // 		else if (str[string_position] == '\0')
// // 		{
// // 			return ;
// // 		}
// // 		else if (g_g_should_exit)
// // 		{
// // 			return ;
// // 		}
// // 		else
// // 		{
// // 			next = init_token();
// // 			token->next = next;
// // 			token = token->next;
// // 			state = SKIP_WHITESPACE;
// // 		}
// // 	}
// // }

static int	handle_read_line_edge_cases(char *line,
		char *prompt, t_token **token)
{
	if (check_syntax(line))
	{
		(*token)->exec = false;
		ft_putendl_fd("basho dice syntax error", 2);
		add_history(line);
		free(prompt);
		free(line);
		line = NULL;
		return (1);
	}
	if (g_should_exit > 0)
	{
		(*token)->exec = false;
		free(prompt);
		free(line);
		return (1);
	}
	return (0);
}

static char	*handle_empty_input(char *line, char *prompt)
{
	while ((!line && !g_should_exit) || !ft_strlen(line))
	{
		if (line)
			free(line);
		line = readline(prompt);
	}
	return (line);
}

static void	_void_return_free_(char *to_free)
{
	free(to_free);
	return ;
}

void	read_line_from_user(t_token **token, t_env_list *env, char **envp)
{
	char	*line;
	char	*prompt;

	line = NULL;
	prompt = get_promt(env);
	line = readline(prompt);
	printf("%i", g_should_exit);
	if (handle_read_line_edge_cases(line, prompt, token))
		return ;
	line = handle_empty_input(line, prompt);
	if (g_should_exit > 0)
	{
		(*token)->exec = false;
		free(prompt);
		free(line);
		return ;
	}
	free(prompt);
	if (!line || g_should_exit)
		return (_void_return_free_(line));
	add_history(line);
	tokenizer(line, *token, env, envp);
	free(line);
	line = NULL;
}
