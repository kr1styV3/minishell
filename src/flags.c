/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coca <coca@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:42:57 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/13 03:55:28 by coca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"


int extract_flag_length(const char *str, int start_pos)
{
	int length;

	length = 0;
	while (ft_isalnum(str[start_pos + length]) || str[start_pos + length] == '-')
		length++;
	return (length);
}

char *allocate_and_copy_flag(const char *str, int start_pos, int length)
{
	char *flag;

	flag = ft_calloc(length + 1, sizeof(char));
	if (!flag)
		return (NULL);
	ft_memcpy(flag, &str[start_pos], length);
	return (flag);
}

int handle_allocation_error(char *str, t_token *token, char *error_message)
{
	free_tokens_line(str, token, error_message);
	return (-1);
}

int assign_flags_to_token_args(t_token *token, char *flags[], int num_flags)
{
	int i;

	i = 0;
	token->arg = ft_recalloc(token->arg, 3 * sizeof(char *), num_flags + 1);
	if (!token->arg)
		return (-1);
	while ( i < num_flags)
	{
		token->arg[i + 1] = flags[i];
		i++;
	}
	token -> arg[i + 1] = NULL;
	return (0);
}

int parse_flags(t_token *token, char *str, int string_position)
{
	char *flags[32];
	int num_flags;
	int offset;

	num_flags = 0;
	offset = 0;
	while (str[string_position + offset] == '-')
	{
		int flag_length = extract_flag_length(str, string_position + offset);
		flags[num_flags] = allocate_and_copy_flag(str, string_position + offset, flag_length);
		if (!flags[num_flags])
			return (handle_allocation_error(str, token, "malloc error"));
		offset += flag_length;
		offset += skip_whitespaces(&str[string_position + offset], NULL);
		num_flags++;
		if (num_flags >= 32)
			return (handle_allocation_error(str, token, "too many flags"));
	}
	if (assign_flags_to_token_args(token, flags, num_flags) != 0)
		return (handle_allocation_error(str, token, "error in loading flags"));
	return (offset);
}


