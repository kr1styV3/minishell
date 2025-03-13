/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coca <coca@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:09:12 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/12 07:34:48 by coca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"
#include "../../headers/parsing.h"


/*
** ft_unsetenv:
** Iterates over the environment list and removes the node whose value starts
** with the provided name followed immediately by '='.
** Returns 0 on success (even if not found) or -1 if the env pointer is NULL.
*/
static int	ft_unsetenv(const char *name, t_env_list **env)
{
	t_env_list	*curr;
	t_env_list	*prev;
	size_t		len;

	if (!env || !*env)
		return (-1);
	len = ft_strlen(name);
	curr = *env;
	prev = NULL;
	while (curr)
	{
		/* Check if the current node's value starts with name and '=' follows */
		if (ft_strncmp(curr->value, name, len) == 0 && curr->value[len] == '=')
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->value);
			free(curr);
			return (0);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

/*
** ft_unset:
** Parses the input string starting at index i to extract the variable name,
** then calls ft_unsetenv to remove the corresponding variable from the env list.
** Updates token->env_ptr with the new environment pointer.
** Returns the number of characters processed, or -1 on error.
*/
int	ft_unset(t_token *token, char *str, int i, t_env_list **env)
{
	int		len;
	char	*name;

	len = skip_whitespaces(&str[i], NULL);
	name = extract_token(&str[i + len]);
	if (!name)
		return (free_tokens_line(str, token, "malloc error"), -1);
	len += ft_strlen(name);
	if (ft_unsetenv(name, env) != 0)
		return (free_tokens_line(str, token, "failed to unset env\n"), -1);
	free(name);
	return (len);
}
