/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/12/09 15:18:16 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../headers/minishell.h"
#include "../headers/parsing.h"
#include "../headers/builtins.h"
#include "../headers/executor.h"
#include "../my_libft/headers/get_next_line.h"
#include "../my_libft/headers/libft.h"
#include "../headers/read_line.h"
#include "pre_executor.h"
#include "../headers/read_line.h"
#include "pre_executor.h"

char *execute_and_capture_output(char *command, char **env)
{
    int pipefd[2];
    pid_t pid;
    int status;
    char *temp;
    char *temp2;

    if (pipe(pipefd) == -1)
        return NULL;

    pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return NULL;
    }
    else if (pid == 0)
    {
        // Child process
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[0]);               // Close unused read end
        close(pipefd[1]);

        // Execute the command
        t_token *token = init_token(); // Initialize your token structure
        tokenizer(command, token, env);
        if (checker(token, env) == 1)
        {
            free_token(token);
            exit(EXIT_FAILURE);
        }
        execute(&token, env); // Execute the command
        free_token(token);
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process
        close(pipefd[1]); // Close unused write end
        temp2 = NULL;
        char *tmp = NULL;
        while (temp != NULL)
        {
            temp = get_next_line(pipefd[0]);
            if (temp)
            {
                if (temp2)
                    tmp = ft_strdup(temp2);
                temp2 = ft_strjoin_until_nl(tmp, temp);
                free(tmp);
                free(temp);
            }

        }
        if (!temp2)
        {
            close(pipefd[0]);
            return NULL;
        }
        close(pipefd[0]);
        waitpid(pid, &status, 0); // Wait for child process
        return temp2;
    }
    return NULL;
}

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

        token->parsed->word = ft_getenv(variable, env);
        len = ft_strlen(variable) + 2; // '{' and '}'
        free(variable);
    }
    else if (str[string_position] == '(')
    {
		char *command = extract_word(&str[string_position + 1], ')');
        if (!command)
            return (free_tokens_line(str, token, "malloc error"), -1);
        len = ft_strlen(command) + 2;  // '(' and ')'

    // Execute the command and capture its output
        char *command_output = execute_and_capture_output(command, env);
        free(command);
        if (!command_output)
            return (free_tokens_line(str, token, "command execution error"), -1);
        ft_strtrim(command_output, " ");
        // Append command_output to output

        token->parsed->word = ft_strdup(command_output);
        free(command_output);
        if (!token->parsed->word)
            return (free_tokens_line(str, token, "malloc error"), -1);
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
        token->parsed->word = ft_getenv(variable, env);
        len = ft_strlen(variable);
        free(variable);
    }
    return len;
}
