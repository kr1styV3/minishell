/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:50:32 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/08 19:00:41 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/builtins.h"
#include "../../headers/parsing.h"
#include "../../headers/env_variables.h"
#include "t_token.h"

char *extract_word_with_dollasign(char *str, char quote, t_token *token, t_env_list *env, char **envp)
{
    int string_position = 0;
    char *word = NULL;
    bool closed_quote = false;

    // Find the position of the next '$' or closing quote
    while (str[string_position] && str[string_position] != quote && str[string_position] != '$')
        string_position++;

    if (str[string_position] == '$')
    {
        // Extract the preceding literal part before '$'
        char *preceding = ft_substr(str, 0, string_position);
        if (!preceding)
            return NULL;

        // Process the variable after '$'
        int var_length = process_variable(&token, str, string_position + 1, env, envp);
        if (var_length == -1)
        {
            free(preceding);
            return NULL;
        }
        string_position += var_length + 1; // +1 to skip '$'

        // Concatenate the preceding literal with the variable's value
        if (token->parsed->word)
        {
            char *temp = ft_strjoin(preceding, token->parsed->word);
            free(preceding);
            if (!temp)
                return NULL;
            preceding = temp;
        }

        // Extract the remaining string after the variable
        char *remaining = ft_substr(str, string_position, ft_strlen(str) - string_position);
        if (!remaining)
        {
            free(preceding);
            return NULL;
        }

        // Concatenate everything to form the final word
        char *result = ft_strjoin(preceding, remaining);
        free(preceding);
        free(remaining);
        return result;
    }

    if (str[string_position] == quote)
        closed_quote = true;

    if (!closed_quote)
        return NULL;

    // Extract the word within quotes
    word = ft_calloc(string_position + 1, sizeof(char));
    if (!word)
        return NULL;
    ft_strncpy(word, str, string_position);
    return word;
}

// int ft_echo(t_token *token, char *str, int i, t_env_list *env, char **envp)
// {
//     int len = skip_whitespaces(&str[i], NULL);
//     bool flag = false;
//     char *word = NULL;
//     char quote;
//     char *output = ft_strdup(""); // Initialize an empty string for output
//     if (!output)
//         return (free_tokens_line(str, token, "malloc error"), -1);

//     // Handle '-n' flag
//     if (str[i + len] == '-' && str[i + len + 1] == 'n')
//     {
//         flag = true;
//         len += 2;
//     }

//     len += skip_whitespaces(&str[i + len], NULL);
//     quote = str[i + len];

//     // Handle different cases based on the first character after options
//     if (quote == '\'')
//     {
//         word = extract_word(&str[i + len + 1], quote);
//         if (!word)
//         {
//             free(output);
//             return (free_tokens_line(str, token, "malloc error"), -1);
//         }
//         len += ft_strlen(word) + 2; // Account for quotes
//         char *temp = ft_strjoin(output, word);
//         free(output);
//         free(word);
//         if (!temp)
//             return (free_tokens_line(str, token, "malloc error"), -1);
//         output = temp;
//     }
//     else if (quote == '\"')
//     {
//         word = extract_word_with_dollasign(&str[i + len + 1], quote, token, env, envp);
//         if (!word)
//         {
//             free(output);
//             return (free_tokens_line(str, token, "malloc error"), -1);
//         }
//         len += ft_strlen(word) + 2; // Account for quotes
//         char *temp = ft_strjoin(output, word);
//         free(output);
//         free(word);
//         if (!temp)
//             return (free_tokens_line(str, token, "malloc error"), -1);
//         output = temp;
//     }
//     else
//     {
//         // Handle unquoted strings, including those with multiple variables
//         while (str[i + len] && str[i + len] != '\'' && str[i + len] != '\"' && str[i + len] != 32)
//         {
//             if (str[i + len] == '$')
//             {
//                 // Process variable
//                 int var_len = process_variable(&token, str, i + len + 1, env, envp);
//                 if (var_len == -1)
//                 {
//                     free(output);
//                     return (free_tokens_line(str, token, "variable processing error"), -1);
//                 }
//                 len += var_len + 1; // +1 for '$'
//                 if (token->parsed->word)
//                 {
//                     char *temp = ft_strjoin(output, token->parsed->word);
//                     free(output);
//                     // free (token->parsed->word);
//                     if (!temp)
//                         return (free_tokens_line(str, token, "malloc error"), -1);
//                     output = temp;
//                 }
//             }
//             else
//             {
//                 // Process literal characters
//                 int start = i + len;
//                 int j = start;
//                 while (str[j] && str[j] != '$' && str[j] != '\'' && str[j] != '\"' && str[j] != 32)
//                     j++;
//                 int substr_len = j - start;
//                 char *substr = ft_substr(str, start, substr_len);
//                 if (!substr)
//                 {
//                     free(output);
//                     return (free_tokens_line(str, token, "malloc error"), -1);
//                 }
//                 len += substr_len;
//                 char *temp = ft_strjoin(output, substr);
//                 free(substr);
//                 free(output);
//                 if (!temp)
//                     return (free_tokens_line(str, token, "malloc error"), -1);
//                 output = temp;
//             }
//         }
//     }
//         token->arg = (char **)ft_calloc(3, sizeof(char *));
//         token->arg[0] = ft_strdup("echo");
//         if (!token->arg[0])
//             free_tokens_line(str, token, "malloc error for internal processes");
//         token->arg[1] = ft_strdup(output);
//         if (!flag)
//             token->arg[1] = ft_freejoin(token->arg[1], "\n");
//         if (!token->arg[1])
//             return (free_tokens_line(str, token, "malloc error for internal processes"), -1);
//         token->arg[2] = NULL;
//         token->checker = false;
//         free(output);
//     return len;
// }
static int	append_to_output(char **output,
		char *addition, char *str, t_token *token)
{
	char	*temp;

	temp = ft_strjoin(*output, addition);
	free(*output);
	if (!temp)
		return (free_tokens_line(str, token, "malloc error"), -1);
	*output = temp;
	return (0);
}

static int	handle_single_quote(char *str, int i, int len,
		t_token *token, char **output)
{
	char	*word;

	word = extract_word(&str[i + len + 1], '\'');
	if (!word)
		return (free_tokens_line(str, token, "malloc error"), -1);
	len += ft_strlen(word) + 2;
	if (append_to_output(output, word, str, token) == -1)
	{
		free(word);
		return (-1);
	}
	free(word);
	return (len);
}

static int	handle_double_quote(char *str, int i, int len,
	t_token *token, char **output,
	t_env_list *env, char **envp)
{
	char	*word;

	word = extract_word_with_dollasign(&str[i + len + 1], '\"', token, env, envp);
	if (!word)
		return (free_tokens_line(str, token, "malloc error"), -1);
	len += ft_strlen(word) + 2;
	if (append_to_output(output, word, str, token) == -1)
	{
		free(word);
		return (-1);
	}
	free(word);
	return (len);
}

static int	handle_unquoted_echo(char *str, int i, int len,
	t_token *token, char **output, t_env_list *env, char **envp)
{
	int	start, j, substr_len;
	char	*substr;

	while (str[i + len] && str[i + len] != '\'' &&
		str[i + len] != '\"' && str[i + len] != ' ')
	{
		if (str[i + len] == '$')
		{
			int var_len = process_variable(&token, str, i + len + 1, env, envp);
			if (var_len == -1)
				return (-1);
			len += var_len + 1;
			if (token->parsed->word)
				if (append_to_output(output, token->parsed->word, str, token) == -1)
					return (-1);
		}
		else
		{
			start = i + len;
			j = start;
			while (str[j] && str[j] != '$' && str[j] != '\'' && str[j] != '\"' && str[j] != ' ')
				j++;
			substr_len = j - start;
			substr = ft_substr(str, start, substr_len);
			if (!substr || append_to_output(output, substr, str, token) == -1)
				return (free(substr), -1);
			free(substr);
			len += substr_len;
		}
	}
	return (len);
}

int	ft_echo(t_token *token, char *str, int i,
	t_env_list *env, char **envp)
{
	int		len;
	char	quote;
	char	*output;
	bool	flag;

	len = skip_whitespaces(&str[i], NULL);
	flag = false;
	output = ft_strdup("");
	if (!output)
		return (free_tokens_line(str, token, "malloc error"), -1);
	if (str[i + len] == '-' && str[i + len + 1] == 'n')
	{
		flag = true;
		len += 2;
	}
	len += skip_whitespaces(&str[i + len], NULL);
	quote = str[i + len];
	if (quote == '\'')
		len = handle_single_quote(str, i, len, token, &output);
	else if (quote == '\"')
		len = handle_double_quote(str, i, len, token, &output, env, envp);
	else
		len = handle_unquoted_echo(str, i, len, token, &output, env, envp);
	if (len == -1 || build_echo_arguments(token, output, flag, str) == -1)
		return (free(output), -1);
	free(output);
	return (len);
}
