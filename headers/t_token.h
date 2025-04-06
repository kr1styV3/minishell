/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:56:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 20:52:45 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_TOKEN_H
# define T_TOKEN_H

# include <stdbool.h>

typedef struct s_operator
{
	char				operator;
	int					fd_overwrite_output;
	int					fd_append_output;
	int					fd_input;
}						t_operator;

typedef struct s_parse
{
	char				*token;
	char				*word;
}						t_parse;

typedef struct s_env_list
{
	char				*value;
	struct s_env_list	*next;
}						t_env_list;

typedef struct s_doc
{
	char				*eof;
	bool				here_doc;
	struct s_doc		*next;
}						t_doc;

typedef struct s_env
{
	void				*env_ptr;
	bool				env_work;
}						t_env;
typedef struct s_token
{
	bool				env_work;
	bool				exec;
	bool				free;
	bool				checker;
	unsigned int		last_exit_status;
	char				**arg;
	struct s_token		*next;
	struct s_operator	*operator;
	struct s_parse		*parsed;
	struct s_env		*env;
	struct s_doc		*doc;
	struct s_doc		**op;
}						t_token;

void	set_token_defaults(t_token *token,
			t_parse *parsed, t_operator *op, t_doc *doc);
void	handle_token_error(t_token *token,
			t_parse *parsed, t_operator *op, t_doc *doc);

#endif
