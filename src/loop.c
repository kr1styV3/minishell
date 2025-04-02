/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:58:56 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/01 13:11:13 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include "env_variables.h"
#include "read_line.h"

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
			(*token)->arg[0] = __ft_strdup_with_free(path);
			ft_free_mtx(paths);
			return (0);
		}
		free(path);
		i++;
	}
	ft_free_mtx(paths);
	return (1);
}

static void	handle_execution(t_token **head, t_token *token,
	t_bau_args meow, int *status)
{
	while (token)
	{
		if (token->checker == true)
		{
			if (checker(&token, meow.meow) == 1)
			{
				free_inside_token("minishell: command not found: ",
					token->parsed->token);
				should_exit = 2;
			}
		}
		token = token->next;
	}
	token = *head;
	if (token->exec == true && !should_exit)
		*status = execute(head, meow.bau_bau, meow.meow);
}

void	shell_loop(t_token **head, t_env_list *_env_ptr,
	char **envp, int *status)
{
	t_token		*token;
	t_bau_args	meow;

	meow.bau_bau = envp;
	meow.meow = _env_ptr;
	while (!should_exit)
	{
		*head = init_token();
		token = *head;
		token->last_exit_status = *status;
		read_line_from_user(head, _env_ptr, envp);
		if (token->exec == true && !should_exit)
			handle_execution(head, token, meow, status);
		if (should_exit == 2)
			should_exit = 0;
		free_token(token);
	}
}
