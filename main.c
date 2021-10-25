#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "main.h"

/**
 * main - simple shell
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nline;
	char **tokens = NULL;
	int status;

	do {
		printf("($) ");
		nline = getline(&line, &len, stdin); /* get line*/
		if (nline == EOF)
			break;

		line[nline - 1] = '\0';
		/* tokenize*/
		tokens = tokenizer(line);
		/* execute*/
		status = execute(tokens);
	} while (status != 1);

	free_all(tokens, len_tokens(line));
	free(line);
	return (0);
}

/**
 * tokenizer - splits str into tokens
 * @str: string to be tokenzed
 * Return: array of token strings
 */
char **tokenizer(char *str)
{
	char **tokens, *token;
	ssize_t length = len_tokens(str), i;

	tokens = malloc((sizeof(char *)) * (length + 1));
	if (tokens == NULL)
		perror("Error");

	token = strtok(str, " ");
	for (i = 0; i < length; i++)
	{
		tokens[i] = malloc(strlen(token) + 1);
		if (tokens[i] == NULL)
		{
			free_all(tokens, i);
			perror("Error");
		}

		strcpy(tokens[i], token);
		token = strtok(NULL, " ");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * len_tokens - length of token
 * @str: string
 * Return: length of token
 */
size_t len_tokens(char *str)
{
	size_t len = 0, i;

	for (i = 0; str[i] != '\0'; i++)
		if (str[i] == ' ')
			len++;

	return (len + 1);
}

/**
 * free_all - frees memory for tokens
 * @tokens: array of string
 * @size: size of token
 */
void free_all(char **tokens, size_t size)
{
	size_t i;

	for (i = 0; i < size; i++)
		free(tokens[i]);

	free(tokens);
}

/**
 * execute - execute a command
 * @args: array of strings
 * Return: status
 */
int execute(char **args)
{
	pid_t my_pid, child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
		return (1);

	if (child_pid == 0)
		if (execve(args[0], args, NULL) == -1)
			perror("execute Error");
		else
			wait(&status);

	return (status);
}
