#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#define DELIMITERS " \t\r\n\a"

// Custom tokenizer function
char **tokenize_input(char *input)
{
    int bufsize = 64;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    int start = 0, end = 0, length = strlen(input);

    if (!tokens)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (end <= length)
    {
        // Check if the current character is a delimiter or end of string
        if (strchr(DELIMITERS, input[end]) || end == length)
        {
            if (end > start)
            {
                token = malloc((end - start + 1) * sizeof(char));
                if (!token)
                {
                    fprintf(stderr, "allocation error\n");
                    exit(EXIT_FAILURE);
                }
                strncpy(token, &input[start], end - start);
                token[end - start] = '\0';
                tokens[position] = token;
                position++;

                if (position >= bufsize)
                {
                    bufsize += 64;
                    tokens = realloc(tokens, bufsize * sizeof(char *));
                    if (!tokens)
                    {
                        fprintf(stderr, "allocation error\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            start = end + 1;
        }
        end++;
    }
    tokens[position] = NULL;
    return tokens;
}

void read_line_from_user()
{
    char command[100];
    char **tokens;

    // Read the entire line of input
    if (fgets(command, sizeof(command), stdin) == NULL)
    {
        perror("fgets");
        exit(EXIT_FAILURE);
    }

    // Remove the newline character if present
    command[strcspn(command, "\n")] = '\0';

    // Tokenize the input command using the custom tokenizer
    tokens = tokenize_input(command);

    // Print tokens for verification
    for (int i = 0; tokens[i] != NULL; i++)
    {
        printf("Token: %s\n", tokens[i]);
        free(tokens[i]); // Free each token after use
    }

    free(tokens); // Free the array of tokens
}