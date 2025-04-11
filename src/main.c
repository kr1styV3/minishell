/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD
/*   Created: 2025/04/06 20:34:25 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 20:36:54 by chrlomba         ###   ########.fr       */
=======
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/03/13 04:44:46 by coca             ###   ########.fr       */
>>>>>>> parent of 7cd5485... did somechange parsing perfect
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
<<<<<<< HEAD
	_env_ptr = env_copy(envp);
	setup_signal_handling();
	shell_loop(head, _env_ptr, envp, &status);
	free_list_copy(_env_ptr);
=======
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
>>>>>>> parent of 7cd5485... did somechange parsing perfect
	free(head);
	rl_clear_history();
	return (0);
}
