/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_checkvar.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 20:33:48 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 20:50:09 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/env_variables.h"
#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	env_replace_entry(t_env_list *env, const char *key, const char *full_entry)
{
	t_env_list	*var;

	var = env_find_var(env, key);
	if (!var)
		return (-1);
	free(var->value);
	var->value = ft_strdup(full_entry);
	if (!var->value)
		return (-1);
	return (0);
}

int	env_add_entry(t_env_list **env, const char *full_entry)
{
	t_env_list		*new_entry;
	t_env_list		*current;

	new_entry = (t_env_list *)malloc(sizeof(t_env_list));
	if (!new_entry)
		return (-1);
	new_entry->value = ft_strdup(full_entry);
	new_entry->next = NULL;
	if (!new_entry->value)
	{
		free(new_entry);
		return (-1);
	}
	if (*env == NULL)
	{
		*env = new_entry;
	}
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new_entry;
	}
	return (0);
}

static char	*build_full_env_entry(t_token *token, char *value)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(token->parsed->token, "=");
	if (!tmp)
		return (NULL);
	result = ft_freejoin(tmp, value);
	return (result);
}

static int	set_or_replace_env(t_env_list **env,
	t_token *token, char *entry)
{
	t_env_list	*existing;

	existing = env_find_var(*env, token->parsed->token);
	if (existing)
	{
		if (env_replace_entry(*env, token->parsed->token, entry) < 0)
			return (var_cleanup(entry, NULL, "Failed to replace env entry\n"));
	}
	else
	{
		if (env_add_entry(env, entry) < 0)
			return (var_cleanup(entry, NULL, "Failed to add env entry\n"));
	}
	return (0);
}

int	check_var(t_token **token, char *line, int *i, t_env_list **env)
{
	char		*var_value;
	char		*entry;

	var_value = extract_value(line, i);
	if (!var_value)
		return (-1);
	entry = build_full_env_entry(*token, var_value);
	if (!entry)
	{
		free(var_value);
		return (-1);
	}
	if (set_or_replace_env(env, *token, entry) < 0)
	{
		free(var_value);
		return (-1);
	}
	free(entry);
	free(var_value);
	return (0);
}
