/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:01:57 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/01 13:08:44 by chrlomba         ###   ########.fr       */
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

int	execute(t_token **token_list, char **env, t_env_list *envp);

#endif
