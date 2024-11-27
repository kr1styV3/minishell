/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:01:57 by chrlomba          #+#    #+#             */
/*   Updated: 2024/11/26 16:59:17 by chrlomba         ###   ########.fr       */
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

void execute(t_token *token_list, char **env);

#endif
