/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:09:12 by chrlomba          #+#    #+#             */
/*   Updated: 2024/09/09 05:23:50 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"
#include "../../headers/parsing.h"

int	ft_unset(t_token *token, char *str, int i)
{
	int		len;
	char	*name;

	len = skip_whitespaces(&str[i], NULL);
	name = extract_token(&str[i + len]);
	if (!name)
		return (free_tokens_line(str, token, "malloc error"), -1);
	len += ft_strlen(name);
	if (unsetenv(name) == -1)
	{
		free(name);
		return (free_tokens_line(str, token, "unset error"), -1);
	}
	free(name);
	return (len);
}
