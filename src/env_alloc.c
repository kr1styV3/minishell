/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 18:24:19 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/10 14:39:12 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/env_variables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* ---------------------------------------------------------
    A helper to "extract" the value from `line` starting at *p_i.
    For demonstration, it:
      - Skips leading spaces/tabs.
      - If there's a double-quote, it reads until the closing quote.
      - Otherwise, it reads until the next whitespace or end of string.
    Adjust this to your real grammar (handling escaped quotes, etc.).
    --------------------------------------------------------- */
char *extract_value(const char *line, int *p_i)
{
     int i;
     int start;
     int end;
     int quoted;
     int length;
     char *val;

     i = *p_i;      /* pull out the caller’s index */
     quoted = 0;    /* 0 = not in quotes, 1 = in quotes */

     /* Skip leading spaces/tabs */
     if (line[i] != '=')
          return NULL;
     i++;
     while (line[i] == ' ' || line[i] == '\t')
     {
          i++;
     }

     /* Check if the value is in quotes */
     if (line[i] == '"')
     {
          quoted = 1;
          i++;
     }

     start = i;

     if (quoted == 1)
     {
          /* read until next quote or end of string */
          while (line[i] != '\0' && line[i] != '"')
          {
                i++;
          }
          end = i;
          if (line[i] == '"')
          {
                i++;  /* skip the closing quote */
          }
     }
     else
     {
          /* read until space/tab or end of string */
          while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t')
          {
                i++;
          }
          end = i;
     }

     length = end - start;
     val = (char *)malloc(length + 1);
     if (val == NULL)
     {
          /* error handling (return NULL) */
          return NULL;
     }
     /* Copy the substring into `val` */
     ft_memcpy(val, &line[start], length);
     val[length] = '\0';

     /* Update the caller's index */
     *p_i = i;

     return val;
}

/* ---------------------------------------------------------
    Find if var_name already exists in env.

    Returns the pointer to the node if found, or NULL if not found.
    --------------------------------------------------------- */

t_env_list *env_find_var(t_env_list *env, const char *var_name)
{
    t_env_list *current;
    size_t key_len;

    if (!env || !var_name)
        return NULL;
    key_len = strlen(var_name);
    current = env;
    while (current)
    {
        /* Check if the entry starts with var_name and is immediately followed by '=' */
        if (ft_strncmp(current->value, var_name, key_len) == 0 &&
            current->value[key_len] == '=')
        {
            return current;  /* Found it */
        }
        current = current->next;
    }
    return NULL;
}


/* ---------------------------------------------------------
    Replace an existing env entry with new_entry.
    We free the old value, then strdup the new one.
    --------------------------------------------------------- */
int env_replace_entry(t_env_list *env, const char *key, const char *full_entry)
{
    t_env_list *var;

    var = env_find_var(env, key);
    if (!var)
        return -1;
    free(var->value);
    var->value = strdup(full_entry);
    if (!var->value)
        return -1;
    return 0;
}


/* ---------------------------------------------------------
    Add a new entry to the env list.
    Returns 0 on success, -1 on failure.
    --------------------------------------------------------- */
int env_add_entry(t_env_list **env, const char *full_entry)
{
    t_env_list *new_entry;
    t_env_list *current;

    new_entry = (t_env_list *)malloc(sizeof(t_env_list));
    if (!new_entry)
        return -1;
    new_entry->value = strdup(full_entry);
    new_entry->next = NULL;
    if (!new_entry->value)
    {
        free(new_entry);
        return -1;
    }
    if (*env == NULL)
    {
        *env = new_entry;
    }
    else
    {
        current = *env;
        while (current->next)
            current = current->next;
        current->next = new_entry;
    }
    return 0;
}


int var_cleanup(char *full_entry, char *var_value, char *str)
{
     write(2, str, ft_strlen(str));
     free(full_entry);
     free(var_value);
     return -1;
}

/* ---------------------------------------------------------
    check_var:
    - `str` might be something like "gigi="
    - we read `line` at index `i` to get the new value (e.g., "lalal")
    - combine to "gigi=lalal"
    - see if "gigi" already in (*envgigi=cazzzzzooooo--------------------------- */
int check_var(t_token **token, char *line, int *i, t_env_list **env)
{
    char *var_value;
    char *full_entry;
    size_t total_len;
    t_env_list *existing_var;

    var_value = extract_value(line, i);
    if (!var_value)
        return -1;
    total_len = strlen((*token)->parsed->token) + 1 + strlen(var_value) + 1; /* key + '=' + value + '\0' */
    full_entry = (char *)malloc(total_len);
    if (!full_entry)
    {
        free(var_value);
        return -1;
    }
    char *tmp = ft_strjoin((*token)->parsed->token, "=");
    full_entry = ft_freejoin(tmp, var_value);

    existing_var = env_find_var(*env, (*token)->parsed->token);
    if (existing_var)
    {
        if (env_replace_entry(*env, (*token)->parsed->token, full_entry) < 0)
            return var_cleanup(full_entry, var_value, "Failed to replace env entry\n");
        (*token)->env_ptr = (void *)*env;
    }
    else
    {
        if (env_add_entry(env, full_entry) < 0)
            return var_cleanup(full_entry, var_value, "Failed to add env entry\n");
        (*token)->env_ptr = (void *)*env;
    }
    free(full_entry);
    free(var_value);
    return 0; /* success */
}

