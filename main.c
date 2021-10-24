#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
	char *line = NULL;
	size_t len = 0, i = 0, j = 0;
	ssize_t nline;
	char **argv = NULL, **temp;

	do
	{
		printf("$cisfun# ");
		nline = getline(&line, &len, stdin);
		char *token = strtok(line, " ");
		while (token)
		{
			/* Allocate space for 2D array argv */
			temp = malloc(sizeof(char **) * ++i);
			if (temp == NULL)
				perror("Error");

			/* copy arg to temp */
			for (j = 0; j < i; j++)
			{
				if (j == i - 1)
				{
					temp[j] = malloc(strlen(token) + 1);
					if (temp[j] == NULL)
						perror("Error");
					temp[j] = token;
				}
				else
					temp[j] = argv[j];
			}
			if (argv != NULL)
				free(argv);
			argv = temp;

			token = strtok(NULL, " ");
		}

		/* Allocate space for 2D array argv */
		temp = malloc(sizeof(char **) * ++i);
		if (temp == NULL)
			return 1;
		for (j = 0; j <= i; j++)
			temp[j] = j == i - 1 ? NULL : argv[j];
		free(argv);
		argv = temp;

		for (j = 0; j < i; j++)
			printf("%s, ", argv[j]);

		if (execve(argv[0], argv, NULL) != -1)
			perror("Error: ");

		// free memory and clear varaibles
		//for (j = 0; j < i; j++)
		//	free(argv[j]);
		free(argv);
		argv = NULL;
		i = 0;
	} while (nline != -1);

	free(line);
	return (0);
}
