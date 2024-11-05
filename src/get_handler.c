/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:47:48 by chrlomba          #+#    #+#             */
/*   Updated: 2024/11/05 13:43:29 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/handler.h"

t_state_handler_map *init_handler(void)
{
    t_state_handler_map *handler_map;

    // Allocate memory for 6 handler mappings
    handler_map = (t_state_handler_map *)malloc(6 * sizeof(t_state_handler_map));
    if (!handler_map)
        return NULL;  // Handle memory allocation failure
    handler_map[0] = (t_state_handler_map){SKIP_WHITESPACE, handle_skip_whitespace};
    handler_map[1] = (t_state_handler_map){IN_BUILTIN, handle_in_builtin};
    handler_map[2] = (t_state_handler_map){IN_WORD, handle_in_word};
    handler_map[3] = (t_state_handler_map){IN_OPERATOR, handle_in_operator};
    handler_map[4] = (t_state_handler_map){IN_VARIABLE, handle_in_variable};
    handler_map[5] = (t_state_handler_map){NORMAL, handle_normal};
	handler_map[6] = (t_state_handler_map){-1, NULL};
    printf("handler size %lu", sizeof(t_state_handler_map));
    return handler_map;
}

state_handler_func get_handler(t_state_handler_map *handler_map, t_state state)
{
    int i;

    // Loop through the handler map to find the matching state
    i = 0;
    while (handler_map[i].handler != NULL)  // Stop when handler is NULL
    {
        if (handler_map[i].state == state)
            return (handler_map[i].handler);
        i++;
    }
    return NULL;  // Return NULL if no handler is found for the state
}
