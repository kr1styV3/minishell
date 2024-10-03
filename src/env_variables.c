/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr> >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:22:24 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/03 17:28:36 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"
#include "../headers/builtins.h"
#include "../headers/executor.h"

int process_variable(t_token *token, char *str, int string_position, char **env)
{
    char *variable;
    int len = 0;

    if (str[string_position] == '{')
    {
        string_position++;
        variable = extract_until_not_alfanum(&str[string_position]);
        if (!variable)
            return (free_tokens_line(str, token, "malloc error"), -1);

        // Check if the closing '}' exists
        if (str[string_position + ft_strlen(variable)] != '}')
        {
            free(variable);
            return (free_tokens_line(str, token, "missing '}'"), -1);
        }

        token->word = ft_getenv(variable, env);
        len = ft_strlen(variable) + 2; // '{' and '}'
        free(variable);
    }
    else if (str[string_position] == '(')
    {
		if (token->token)
			free(token->token);
        token->token = extract_word(&str[string_position + 1], ')');
		tokenizer(token->token, token, env);
        if(checker(token, env) == -1)
			return (free_inside_token(token, "minishell: command not found: ", token->token), -1);
		execute(token);
        len = ft_strlen(token->token) + 2; // '(' and ')'
    }
    else if (str[string_position] == '?')
    {
        ft_putnbr_fd(token->last_exit_status, STDOUT_FILENO);
        return 1; // Only '?' is one character
    }
    else
    {
        variable = extract_until_not_alfanum(&str[string_position]);
        if (!variable)
            return (free_tokens_line(str, token, "malloc error"), -1);
        token->word = ft_getenv(variable, env);
        len = ft_strlen(variable);
        free(variable);
    }
    return len;
}
