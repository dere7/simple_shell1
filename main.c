#include "main.h"

/**
 * main - simple shell
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 on success
 */
int main(void)
{
	while (1)
	{
		char input[SIZE + 1] = {0x0};
		char *arguments[ARGUMENTS_MAX + 1] = {NULL};
		char tty_flag;
		int i;
		char *pointer = input;
		int current_status;

		tty_flag = (char) isatty(STDIN_FILENO);

		if (tty_flag == 1)
			printf("%s ", getuid() == 0 ? "#" : "$");
		fgets(input, SIZE, stdin);

		if (*pointer == '\n')
			continue;

		for (i = 0; *pointer && i < (int) sizeof(arguments) ; pointer++)
		{
			if (*pointer == '\n')
				break;
			if (*pointer == ' ')
				continue;
			for (arguments[i++] = pointer; *pointer != '\n' && *pointer &&
										   *pointer != ' '; pointer++)
				;
			*pointer = '\0';
		}

		if (compare_strs(COMMAND_EXIT, arguments[0]) == 0)
			return (0);
		signal(SIGINT, SIG_DFL);

		if (fork() == 0)
			exit(execvp(arguments[0], arguments));
		signal(SIGINT, SIG_IGN);

		wait(&current_status);
	}
}
