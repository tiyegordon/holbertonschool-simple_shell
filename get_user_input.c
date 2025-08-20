#include "main.h"

/**
 * get_arguments_from_input - creates an array of strings containing arguments
 * @buffer: buffer containing user input
 * @arguments: pointer to arguments array to be populated
 * @size: maximum size of the arguments array
 * Return: argument array
 */
char **get_arguments_from_input(char *buffer, char **arguments, size_t size)
{
	int i = 0;
	char *token;

	token = strtok(buffer, " ");
	while (token != NULL)
	{
		if (i >= (int)(size - 1))
		{
			size = size * 2;
			arguments = realloc(arguments, size * sizeof(char *));
			if (!arguments)
			{
				perror("realloc");
				free(buffer);
				free_memory(arguments);
				exit(EXIT_FAILURE);
			}
		}

		arguments[i] = strdup(token);
		if (!arguments[i])
		{
			perror("strdup");
			free(buffer);
			free_memory(arguments);
			exit(EXIT_FAILURE);
		}

		token = strtok(NULL, " ");
		i++;
	}

	arguments[i] = NULL;

	return (arguments);
}

/**
 * get_user_input - uses getline to accept user input
 * Return: user input
 */
char **get_user_input()
{
	char *buffer = NULL;
	char **arguments;
	size_t n = 0, size = 32;

	if (getline(&buffer, &n, stdin) == -1)
	{
		free(buffer);
		/* exit on CTRL+D */
		exit(0);
	}

	arguments = malloc(sizeof(char *) * size);

	if (arguments == NULL)
	{
		perror("malloc");
		free(buffer);
		exit(EXIT_FAILURE);
	}

	/* removing trailing newline if present */
	if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] == '\n')
		buffer[strlen(buffer) - 1] = '\0';

	arguments = get_arguments_from_input(buffer, arguments, size);

	free(buffer);
	return (arguments);
}
