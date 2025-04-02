/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/03/28 15:24:01 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Updated to use t_env_list instead of char **env    +:+     Updated: 2025/03/10  */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"
#include "../headers/builtins.h"
#include "../headers/executor.h"
#include "../my_libft/headers/get_next_line.h"
#include "../my_libft/headers/libft.h"
#include "../headers/read_line.h"
#include "env_variables.h"

/*
** execute_and_capture_output()
** Executes a command in a child process while redirecting its stdout into a pipe.
** The command is tokenized and executed using your shell’s routines.
** 'env' is now passed as a t_env_list pointer.
*/
#include <stdlib.h>
#include <string.h>  // For standard strdup if not using ft_strdup

/*
** env_list_to_array:
** Counts the nodes in the t_env_list, allocates an array of char * with count+1 entries,
** duplicates each value into the array, and sets the last pointer to NULL.
**
** Returns the newly allocated char ** array on success or NULL on failure.
*/

/*
** ft_getenv:
** Searches the environment linked list for an entry matching 'key'.
** The 'value' field in each node is expected to be in the form "KEY=value".
** If found, returns a newly allocated copy of the substring after '='.
** If not found, returns a newly allocated empty string.
*/
char *ft_getenv(const char *key, t_env_list *env)
{
    size_t key_len;
    t_env_list *current;

    if (!key)
        return (ft_strdup(""));

    key_len = ft_strlen(key);
    current = env;
    while (current)
    {
        if (ft_strncmp(current->value, key, key_len) == 0 && current->value[key_len] == '=')
            return (ft_strdup(current->value + key_len + 1));
        current = current->next;
    }
    return (ft_strdup(""));
}
/*
static char **env_list_to_array(t_env_list *env)
{
    int         count = 0;
    t_env_list  *current = env;
    char        **env_array;
    int         i;

    // Count the number of nodes in the list
    while (current)
    {
        count++;
        current = current->next;
    }
    // Allocate the array (with one extra element for the NULL terminator)
    env_array = malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);

    // Duplicate each environment string into the array
    current = env;
    i = 0;
    while (current)
    {
        env_array[i] = ft_strdup(current->value);
        if (!env_array[i])
        {
            // Free any previously allocated strings on failure
            while (i > 0)
            {
                i--;
                free(env_array[i]);
            }
            free(env_array);
            return (NULL);
        }
        i++;
        current = current->next;
    }
    env_array[i] = NULL;
    return env_array;
}
    */

char *execute_and_capture_output(char *command, t_env_list *env, char **envp)
{
    int     pipefd[2];
    pid_t   pid;
    int     status;
    char    *temp;
    char    *temp2;
    char    *tmp;

    if (pipe(pipefd) == -1)
        return (NULL);
    pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (NULL);
    }
    else if (pid == 0)
    {
        /* Child process: redirect stdout to pipe and execute command */
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        t_token *token = init_token(); // Initialize your token structure
        tokenizer(command, token, env, envp);
        if (token->checker)
        {
            if (checker(&token, env) == 1)
            {
                ft_putstr_fd("invalid command expansion : ", 2);
                ft_putstr_fd(token->parsed->token, 2);
                free_token(token);
                should_exit = 1;
                return(NULL);
            }
        }
        execute(&token, envp, env);
        free_token(token);
        exit(EXIT_SUCCESS);
    }
    else
    {
        /* Parent process: read the command output from the pipe */
        close(pipefd[1]);
        temp2 = NULL;
        while (1)
        {
            temp = get_next_line(pipefd[0]);
            if (!temp)
                break;
            if (temp2)
            {
                tmp = ft_strdup(temp2);
                free(temp2);
            }
            else
                tmp = ft_strdup("");
            temp2 = ft_strjoin_until_nl(tmp, temp);
            free(tmp);
            free(temp);
        }
        close(pipefd[0]);
        waitpid(pid, &status, 0);
        return (temp2);
    }
    return (NULL);
}

/*
** process_variable()
** Processes a variable found in the input string and assigns the corresponding
** value from your environment list into token->parsed->word.
** The function now works with a t_env_list pointer.
*/int process_variable(t_token **token, char *str, int string_position, t_env_list *env, char **envp)
{
    char *variable;
    int   len = 0;

    if (str[string_position] == '{')
    {
        string_position++;
        variable = extract_until_not_alfanum(&str[string_position]);
        if (!variable)
            return (free_tokens_line(str, *token, "malloc error"), -1);
        if (str[string_position + ft_strlen(variable)] != '}')
        {
            free(variable);
            return (free_tokens_line(str, *token, "missing '}'"), -1);
        }
        (*token)->parsed->word = ft_getenv(variable, env);
        len = ft_strlen(variable) + 2; /* accounting for '{' and '}' */
        free(variable);
    }
    else if (str[string_position] == '(')
    {
        char *command = extract_word(&str[string_position + 1], ')');
        if (!command)
            return (free_tokens_line(str, *token, "malloc error"), -1);
        len = ft_strlen(command) + 2;  /* accounting for '(' and ')' */
        /* Execute the command and capture its output */
        char *command_output = execute_and_capture_output(command, env, envp);
        free(command);
        if (!command_output)
            return (free_tokens_line(str, *token, "command execution error"), -1);
        char *tmp = ft_strtrim(command_output, " ");
        free(command_output);
        (*token)->parsed->word = ft_strdup(tmp);
        free(tmp);
        if (!(*token)->parsed->word)
            return (free_tokens_line(str, *token, "malloc error"), -1);
    }
    else if (str[string_position] == '?')
    {
        ft_putnbr_fd((*token)->last_exit_status, STDOUT_FILENO);
        return (1); /* Only '?' is one character */
    }
    else
    {
        variable = extract_until_not_alfanum(&str[string_position]);
        if (!variable)
            return (free_tokens_line(str, *token, "malloc error"), -1);
        (*token)->parsed->word = ft_getenv(variable, env);
        len = ft_strlen(variable);
        free(variable);
    }
    return (len);
}

