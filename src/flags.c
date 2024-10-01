/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr> >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:42:57 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/01 18:29:51 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"

int	parse_flags(t_token *token, char *str, int string_position)
{
	int length;
	int mtx_pos;
	int args_pos;
	int return_value;
	char *tmp_flags[32];

	mtx_pos = 1;
	args_pos = 0;
	return_value = 0;
	while (1)
	{
		length = 0;
		while (ft_isalnum(str[string_position + return_value + length]) || str[string_position + return_value + length] == '-')
			length++;
		tmp_flags[args_pos] = ft_calloc(length + 1, sizeof(char));
		if (!tmp_flags[args_pos])
			return (free_tokens_line(str, token, "malloc error"), -1);
		ft_memcpy(tmp_flags[args_pos], &str[string_position + return_value], length);
		length += skip_whitespaces(&str[string_position + return_value + length], NULL);
		return_value += length;
		if (str[string_position + return_value] != '-')
			break ;
		args_pos++;
	}
	free(token->args);
	token->args = ft_calloc(args_pos + 2, sizeof(char *));
	if (!token->args)
		return (free_tokens_line(str, token, "malloc error"), -1);
	while (mtx_pos -1 <= args_pos)
	{
		token->args[mtx_pos] = tmp_flags[mtx_pos - 1];
		mtx_pos++;
	}
	return (return_value);
}
