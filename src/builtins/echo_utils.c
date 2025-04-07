/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:59:25 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 18:59:53 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/builtins.h"
#include "../../headers/parsing.h"
#include "../../headers/env_variables.h"
#include "t_token.h"

int	build_echo_arguments(t_token *token,
		char *output, bool flag, char *str)
{
	token->arg = ft_calloc(3, sizeof(char *));
	if (!token->arg)
		return (free_tokens_line(str, token, "malloc error"), -1);
	token->arg[0] = ft_strdup("echo");
	if (!token->arg[0])
		return (free_tokens_line(str, token, "malloc error"), -1);
	token->arg[1] = ft_strdup(output);
	if (!token->arg[1])
		return (free_tokens_line(str, token, "malloc error"), -1);
	if (!flag)
	{
		token->arg[1] = ft_freejoin(token->arg[1], "\n");
		if (!token->arg[1])
			return (free_tokens_line(str, token, "malloc error"), -1);
	}
	token->arg[2] = NULL;
	token->checker = false;
	return (0);
}
