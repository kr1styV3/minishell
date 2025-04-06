/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:01:57 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 20:26:37 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

typedef enum e_exec_state
{
	EXEC_START,
	EXEC_CMD,
	EXEC_PIPE,
	EXEC_REDIRECTION,
	EXEC_BACKGROUND,
	EXEC_END
}	t_exec_state;

typedef struct s_env_pass
{
	t_env_list	*meow;
	char		**bau_bau;
}				t_bau_args;

int		execute(t_token **token_list, char **env, t_env_list *envp);
int		handle_here_docs(t_token *current, t_env_list *env, char **envp);
int		execute_pipeline(t_token *job_start, t_token *job_end,
			char **env, t_env_list *envp);
void	setup_redirections(t_token *current);
pid_t	fork_and_exec(t_token *current, int prev_fd, int *pipe_fd,
			bool is_piped, char **env, t_env_list *envp);

#endif
