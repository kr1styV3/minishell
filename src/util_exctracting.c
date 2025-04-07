/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_exctracting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:54:13 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 18:54:45 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "builtins.h"
#include "t_token.h"

char	*extract_until_not_alfanum(char *str)
{
	int		i;
	int		k;
	char	*word;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == ' ' || str[i] == '_'))
		i++;
	word = ft_substr(str, 0, i);
	if (!word)
		return (NULL);
	i = 0;
	k = 0;
	while (word[i])
	{
		if (word[i] == ' ')
		{
			word[k++] = ' ';
			while (word[i] == ' ')
				i++;
		}
		else
			word[k++] = word[i++];
	}
	word[k] = '\0';
	return (word);
}
