/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:09:06 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 20:06:52 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env_list(t_env_list *list)
{
	t_env_list	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free(temp->value);
		free(temp);
	}
}

static int	_return_free_(t_env_list *to_free)
{
	free(to_free);
	return (1);
}

static int	fill_env_list(t_env_list **head, char **envp)
{
	t_env_list	*current;
	t_env_list	*new_node;
	int			i;

	i = 0;
	current = NULL;
	while (envp[i])
	{
		new_node = malloc(sizeof(t_env_list));
		if (!new_node)
			return (1);
		new_node->value = ft_strdup(envp[i]);
		if (!new_node->value)
			return (_return_free_(new_node));
		new_node->next = NULL;
		if (!*head)
			*head = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
	return (0);
}

t_env_list	*env_copy(char **envp)
{
	t_env_list	*head;

	head = NULL;
	if (!envp)
		return (NULL);
	if (fill_env_list(&head, envp))
	{
		free_env_list(head);
		return (NULL);
	}
	return (head);
}

void	free_list_copy(t_env_list *ptr)
{
	t_env_list	*temp;

	while (ptr)
	{
		temp = ptr->next;
		if (ptr->value)
			free(ptr->value);
		free(ptr);
		ptr = temp;
	}
}
