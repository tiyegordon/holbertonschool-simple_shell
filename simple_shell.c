#include <signal.h>
#include "main.h"

static int exit_status;

/**
 * handle_sigint - Functions that handles "CTRL + C " command
 * @sig: Signal number (explicitly unused in this fucntion)
 */

void handle_sigint(int sig)
{
	(void)sig;
	printf("\n#cisfun$ ");
	fflush(stdout);
}

/**
 * handle_space - Functions that forks and executes process
 * @str: String to check
 * Return: False if the command has at least one character
 * different than " " or "tab", True if otherwise
 */

bool handle_space(char *str)
{
	int i = 0;

	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\t')
			/* If at least one character is not tab or " " */
			return (false);
		i++;
	}
	/* If all character are either tab or " " */
	return (true);

}

/**
 * print_env - print environment
 * @env: environment
 * Return: nothing
 */
void print_env(char **env)
{
	int i = 0;

	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
}

/**
 * execute - forks and executes process
 * @arguments: arguments to execute
 * @env: environment variables of system
 */
void execute(char **arguments, char **env)
{
	char *command;
	int status;
	pid_t pid;

	if (arguments[0] == NULL)
		return;

	if (arguments[0][0] == '/' || arguments[0][0] == '.')
		command = arguments[0];
	else
	{
		command = search_path(arguments[0], env);
		if (command == NULL)
		{
			fprintf(stderr, "%s: Command not found\n", arguments[0]);
			return;
		}
	}

	pid = fork();
	if (pid < 0)
		perror("Error");
	else if (pid == 0)
	{
		execve(command, arguments, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else
			exit_status = 1;
	}
}

/**
 * main - main function
 * @argc: argument count
 * @argv: argument vector
 * @env: environment variables of system
 * Return: Always 0.
 */
int main
	(__attribute__((unused)) int argc,
	__attribute__((unused)) char *argv[],
	char **env)
{
	char **arguments;

	signal(SIGINT, handle_sigint);
	while (1)
	{
		/* in interactive mode */
		if (isatty(0))
			printf("#cisfun$ ");

		arguments = get_user_input();
		if (
			arguments == NULL ||
			arguments[0] == NULL ||
			handle_space(arguments[0])
		)
		{
			free_memory(arguments);
			continue;
		}

		if (strcmp(arguments[0], "exit") == 0)
		{
			free_memory(arguments);
			return (exit_status);
		}

		if (strcmp(arguments[0], "env") == 0)
		{
			print_env(env);
		} else
			execute(arguments, env);

		free_memory(arguments);
	}

	return (0);
}
