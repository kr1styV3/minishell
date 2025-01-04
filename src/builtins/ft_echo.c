/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/01/04 11:46:31 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../headers/builtins.h"
#include "../../headers/parsing.h"
#include "../../headers/env_variables.h"
#include "t_token.h"

#define OPERATORS "<>|&-"

char	*extract_until_not_alfanum(char *str)
{
	int		i;
	int		k;
	char	*word;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == ' '))
		i++;
	word = ft_substr(str, 0, i);
	if (!word)
		return (NULL);
	i = 0;
	k = 0;
	while (word[i])
	{
		if (word[i] == ' ')
		{
			word[k++] = ' ';
			while (word[i] == ' ')
				i++;
		}
		else
			word[k++] = word[i++];
	}
	word[k] = '\0';
	return (word);
}

char *extract_word_with_dollasign(char *str, char quote, t_token *token, char **env)
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
        int var_length = process_variable(&token, str, string_position + 1, env);
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

int ft_echo(t_token *token, char *str, int i, char **env)
{
    int len = skip_whitespaces(&str[i], NULL);
    bool flag = false;
    char *word = NULL;
    char quote;
    char *output = ft_strdup(""); // Initialize an empty string for output
    if (!output)
        return (free_tokens_line(str, token, "malloc error"), -1);

    // Handle '-n' flag
    if (str[i + len] == '-' && str[i + len + 1] == 'n')
    {
        flag = true;
        len += 2;
    }

    len += skip_whitespaces(&str[i + len], NULL);
    quote = str[i + len];

    // Handle different cases based on the first character after options
    if (quote == '\'')
    {
        word = extract_word(&str[i + len + 1], quote);
        if (!word)
        {
            free(output);
            return (free_tokens_line(str, token, "malloc error"), -1);
        }
        len += ft_strlen(word) + 2; // Account for quotes
        char *temp = ft_strjoin(output, word);
        free(output);
        free(word);
        if (!temp)
            return (free_tokens_line(str, token, "malloc error"), -1);
        output = temp;
    }
    else if (quote == '\"')
    {
        word = extract_word_with_dollasign(&str[i + len + 1], quote, token, env);
        if (!word)
        {
            free(output);
            return (free_tokens_line(str, token, "malloc error"), -1);
        }
        len += ft_strlen(word) + 2; // Account for quotes
        char *temp = ft_strjoin(output, word);
        free(output);
        free(word);
        if (!temp)
            return (free_tokens_line(str, token, "malloc error"), -1);
        output = temp;
    }
    else
    {
        // Handle unquoted strings, including those with multiple variables
        while (str[i + len] && str[i + len] != '\'' && str[i + len] != '\"')
        {
            if (str[i + len] == '$')
            {
                // Process variable
                int var_len = process_variable(&token, str, i + len + 1, env);
                if (var_len == -1)
                {
                    free(output);
                    return (free_tokens_line(str, token, "variable processing error"), -1);
                }
                len += var_len + 1; // +1 for '$'
                if (token->parsed->word)
                {
                    char *temp = ft_strjoin(output, token->parsed->word);
                    free(output);
                    free (token->parsed->word);
                    if (!temp)
                        return (free_tokens_line(str, token, "malloc error"), -1);
                    output = temp;
                }
            }
            else
            {
                // Process literal characters
                int start = i + len;
                int j = start;
                while (str[j] && str[j] != '$' && str[j] != '\'' && str[j] != '\"')
                    j++;
                int substr_len = j - start;
                char *substr = ft_substr(str, start, substr_len);
                if (!substr)
                {
                    free(output);
                    return (free_tokens_line(str, token, "malloc error"), -1);
                }
                len += substr_len;
                char *temp = ft_strjoin(output, substr);
                free(substr);
                free(output);
                if (!temp)
                    return (free_tokens_line(str, token, "malloc error"), -1);
                output = temp;
            }
        }
    }
    if (ft_strchr(OPERATORS, str[i + len + 1]))
    {
       int operator_len = process_operator(&token, str, i + len + 1, NULL);
       if (token->operator->fd_append_output > 0)
            ft_putstr_fd(output, token->operator->fd_append_output);
        else if (token->operator->fd_overwrite_output > 0)
            ft_putstr_fd(output, token->operator->fd_overwrite_output);
        else if (token->operator->fd_input > 0)
            ft_putstr_fd(output, token->operator->fd_input);
        len += operator_len + 1;
    }
    else
    {
        ft_putstr_fd(output, 1);
        if (!flag)
            ft_putchar_fd('\n', 1);
        free(output);
    }
    return len;
}
