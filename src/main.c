/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coca <coca@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/03/13 04:44:46 by coca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "parsing.h"
#include "builtins.h"
#include "executor.h"
#include "read_line.h"
#include "t_token.h"
#include "../my_libft/headers/libft.h"
#include "env_variables.h"
volatile sig_atomic_t should_exit = 0;

void handle_sigint(int sig)
{
	(void)sig;
	should_exit = 2;
}

void handle_sigterm(int sig)
{
	(void)sig;
	should_exit = 1;
}

void handle_sigquit(int sig)
{
	(void)sig;
}

void setup_signal_handling()
{
	struct sigaction sa_int;
	struct sigaction sa_term;
	struct sigaction sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_term.sa_handler = handle_sigterm;
	sigemptyset(&sa_term.sa_mask);
	sa_term.sa_flags = 0;
	sigaction(SIGTERM, &sa_term, NULL);
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

int	checker(t_token **token, t_env_list *envp)
{
	char	*path;
	char	**paths;
	char	*part_path;
	int		i;

	i = 0;
	path = ft_getenv("PATH", envp);
	paths = ft_split(path, ':');
	free(path);
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, (*token)->parsed->token);
		free(part_path);
		if (access(path, X_OK) == 0)
		{
			(*token)->arg[0] = ft_strdup(path);
			free(path);
			ft_free_mtx(paths);
			return (0);
		}
		free(path);
		i++;
	}
	ft_free_mtx(paths);
	return (1);
}

t_env_list	*env_list(char **envp)
{
	t_env_list	*env_ptr;
	t_env_list	*tmp;
	int			i;

	i = 0;
	env_ptr = (t_env_list *)malloc(sizeof(t_env_list));
	if (!env_ptr)
		ft_error("Failed to allocate memory for env_ptr.");
	env_ptr->value = ft_strdup(envp[i]);
	env_ptr->next = NULL;
	tmp = env_ptr;
	i++;
	while (envp[i])
	{
		tmp->next = (t_env_list *)malloc(sizeof(t_env_list));
		if (!tmp->next)
			ft_error("Failed to allocate memory for env_ptr.");
		tmp = tmp->next;
		tmp->value = ft_strdup(envp[i]);
		tmp->next = NULL;
		i++;
	}
	return (env_ptr);
}
t_env_list *env_copy(char **envp)
{
    t_env_list *head = NULL;
    t_env_list *current = NULL;
    t_env_list *new_node;
    int i = 0;

    if (!envp)
        return (NULL);
    while (envp[i])
    {
        new_node = (t_env_list *)malloc(sizeof(t_env_list));
        if (!new_node)
        {
            // Free the already allocated list on error
            t_env_list *temp;
            while (head)
            {
                temp = head;
                head = head->next;
                free(temp->value);
                free(temp);
            }
            return (NULL);
        }
        new_node->value = ft_strdup(envp[i]);
        if (!new_node->value)
        {
            free(new_node);
            t_env_list *temp;
            while (head)
            {
                temp = head;
                head = head->next;
                free(temp->value);
                free(temp);
            }
            return (NULL);
        }
        new_node->next = NULL;
        if (!head)
        {
            head = new_node;
            current = new_node;
        }
        else
        {
            current->next = new_node;
            current = new_node;
        }
        i++;
    }
    return (head);
}


int main(int ac, char **av, char **envp)
{
	t_token		**head;
	t_token		*token;
	t_env_list	*_env_ptr;
	int status = -1;

	_env_ptr = env_copy(envp);
	head = (t_token **)malloc(sizeof(t_token *));
	if (!head)
		ft_error("Failed to allocate memory for token.");
	// setup_signal_handling();
	(void)ac;
	(void)av;
	while (!should_exit)
	{
		*head = init_token();
		token = *head;
		printf("last exitn status: %d\n", status);
		token->last_exit_status = status;
		read_line_from_user(&(*head), _env_ptr);
		if (token->exec == true || should_exit)
		{
			while (token)
			{
				if (token->parsed->token)
				{
					if (checker(&token, _env_ptr) == 1 && !token->checker)
					{
						free_inside_token("minishell: command not found: ", token->parsed->token);
						token->exec = false;
					}
					printf("%s\n", token->arg[1]);
				}
				token = token->next;
			}
			token = *head;
			if (token->exec == true)
				status = execute(head, envp, _env_ptr);
		}
		if (should_exit == 2)
			should_exit = 0;
		free_token(token);
	}
	free(head);
	rl_clear_history();
	return 0;
}
