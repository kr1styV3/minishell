/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:10:26 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/23 13:47:01 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"

int	ft_pwd(t_token *token)
{
	char	cwd[4096];
	char	*print;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (-1);
	}
	print = ft_strjoin(cwd, "\n");
	token->arg = (char **)ft_calloc(3, sizeof(char *));
	token->arg[1] = __ft_strdup_with_free(print);
	if (!token->arg[1])
		return (free_tokens_line(NULL, token,
				"malloc eroor for internal process"), 0);
	token->arg[0] = ft_strdup("pwd");
	if (!token->arg[0])
		return (free_tokens_line(NULL, token,
				"malloc eroor for internal process"), 0);
	token->arg[2] = NULL;
	token->checker = false;
	return (0);
}
