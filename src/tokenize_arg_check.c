/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:04:25 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/04 19:29:41 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "builtins.h"

static int	ft_handle_operator_or_end(t_token **token,
		char *str, int i, int start_pos)
{
	if (ft_strchr(OPERATORS, str[i]) && (*token)->parsed->token == NULL)
	{
		(*token)->checker = false;
		return (i - start_pos);
	}
	if (str[i] == '\0')
		return (i - start_pos);
	return (-1);
}

static void	ft_allocate_args(t_token **token, int arg_count)
{
	if (arg_count == 0)
	{
		(*token)->arg = (char **)ft_calloc(2, sizeof(char *));
		return ;
	}
	(*token)->arg = (char **)ft_calloc(arg_count + 2, sizeof(char *));
	(*token)->arg[0] = NULL;
}

static int	ft_calculate_final_position(char *str, int i, int arg_count)
{
	int	fill_pos;
	int	consumed_args;

	fill_pos = i;
	consumed_args = 0;
	while (str[fill_pos] && consumed_args < arg_count)
	{
		fill_pos += skip_whitespaces(&str[fill_pos], NULL);
		if (!str[fill_pos] || ft_strchr(OPERATORS, str[fill_pos]))
			break ;
		get_next_arg_len(str, &fill_pos);
		consumed_args++;
	}
	return (fill_pos);
}

static void	fill_arguments(t_token *token,
		const char *str, int start, int count)
{
	int	arg_i;
	int	pos;

	arg_i = 1;
	pos = start;
	while (str[pos] && arg_i <= count)
	{
		pos += skip_whitespaces((char *)&str[pos], NULL);
		if (!str[pos] || ft_strchr(OPERATORS, str[pos]))
			break ;
		if (str[pos] == '\'' || str[pos] == '\"')
			pos = fill_quoted_argument(token, str, pos, arg_i);
		else
			pos = fill_unquoted_argument(token, str, pos, arg_i);
		arg_i++;
	}
}

int	ft_checkwordarg(t_token **token, char *str, int i)
{
	int	start_pos;
	int	arg_count;
	int	final_pos;

	start_pos = i;
	if (ft_isbuiltin((*token)->parsed->token))
		return (0);
	i += skip_whitespaces(&str[i], NULL);
	final_pos = ft_handle_operator_or_end(token, str, i, start_pos);
	if (final_pos != -1)
		return (final_pos);
	arg_count = count_arguments(str, i);
	ft_allocate_args(token, arg_count);
	if (arg_count == 0)
		return (i - start_pos);
	fill_arguments(*token, str, i, arg_count);
	i = ft_calculate_final_position(str, i, arg_count);
	return (i - start_pos);
}
