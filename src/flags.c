/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr> >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:42:57 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/01 19:02:46 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"


int	extract_flag_length(const char *str, int start_pos)
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

int assign_flags_to_token_args(t_token *token, char *tmp_flags[], int args_pos)
{
	int mtx_pos;

	mtx_pos = 1;
	while (mtx_pos - 1 <= args_pos)
	{
		token->args[mtx_pos] = tmp_flags[mtx_pos - 1];
		mtx_pos++;
	}
	return (0);
}


int parse_flags(t_token *token, char *str, int string_position)
{
	int length;
	int args_pos;
	int return_value;
	char *tmp_flags[32];

	args_pos = 0;
	return_value = 0;
	while (1)
	{
		length = extract_flag_length(str, string_position + return_value);
		tmp_flags[args_pos] = allocate_and_copy_flag(str, string_position + return_value, length);
		if (!tmp_flags[args_pos])
			return (handle_allocation_error(str, token, "malloc error"));
		length += skip_whitespaces(&str[string_position + return_value + length], NULL);
		return_value += length;
		if (str[string_position + return_value] != '-')
			break;
		args_pos++;
	}
	token->args = ft_recalloc(token->args, 2, args_pos + 2);
	if (!token->args)
		return (handle_allocation_error(str, token, "malloc error"));
	if (assign_flags_to_token_args(token, tmp_flags, args_pos) != 0)
		return (handle_allocation_error(str, token, "error in loading flags"));
	return (return_value);
}

