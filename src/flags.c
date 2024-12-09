/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:42:57 by chrlomba          #+#    #+#             */
/*   Updated: 2024/12/06 17:07:46 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"


int extract_flag_length(const char *str, int start_pos)
{
    int length = 0;

    while (ft_isalnum(str[start_pos + length]) || str[start_pos + length] == '-')
        length++;
    return (length);
}

char *allocate_and_copy_flag(const char *str, int start_pos, int length)
{
    char *flag = ft_calloc(length + 1, sizeof(char));
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

    // Reallocate memory for the args array to fit all flags
    token->arg = ft_recalloc(token->arg, 2 * sizeof(char *), num_flags + 1); // Keep args[0] empty
    if (!token->arg)
        return (-1);

    // Assign flags starting from token->args[1]
    for (i = 0; i < num_flags; i++)
    {
        token->arg[i + 1] = flags[i];
    }
    return (0);
}

int parse_flags(t_token *token, char *str, int string_position)
{
    char *flags[32];  // Temporary storage for up to 32 flags
    int num_flags = 0;
    int offset = 0;

    while (str[string_position + offset] == '-') // Loop until no more flags are found
    {
        int flag_length = extract_flag_length(str, string_position + offset);
        flags[num_flags] = allocate_and_copy_flag(str, string_position + offset, flag_length);
        if (!flags[num_flags])
            return (handle_allocation_error(str, token, "malloc error"));

        offset += flag_length;
        offset += skip_whitespaces(&str[string_position + offset], NULL);
        num_flags++;

        if (num_flags >= 32)  // Avoid buffer overflow for tmp_flags
        {
            return (handle_allocation_error(str, token, "too many flags"));
        }
    }

    // Assign parsed flags to token->args
    if (assign_flags_to_token_args(token, flags, num_flags) != 0)
        return (handle_allocation_error(str, token, "error in loading flags"));
    return (offset);
}


