/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:55:24 by chrlomba          #+#    #+#             */
/*   Updated: 2024/12/06 16:31:32 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_VARIABLES_H
# define ENV_VARIABLES_H

# include "minishell.h"


int process_variable(t_token **token, char *str, int string_position, char **env);
char *execute_and_capture_output(char *command, char **env);



#endif
