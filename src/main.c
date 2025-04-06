/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:34:25 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 20:36:54 by chrlomba         ###   ########.fr       */
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

volatile sig_atomic_t	g_should_exit = 0;

int	main(int ac, char **av, char **envp)
{
	t_token		**head;
	t_env_list	*_env_ptr;
	int			status;

	(void)ac;
	(void)av;
	status = -1;
	head = (t_token **)malloc(sizeof(t_token *));
	if (!head)
		ft_error("Failed to allocate memory for token.");
	_env_ptr = env_copy(envp);
	setup_signal_handling();
	shell_loop(head, _env_ptr, envp, &status);
	free_list_copy(_env_ptr);
	free(head);
	rl_clear_history();
	return (0);
}
