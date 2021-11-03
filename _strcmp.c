#include "main.h"
/**
 * compare_strs - writes the character c to stdout
 * @s1: The character to print
 * @s2: Second character
 * On error, -1 is returned, and errno is set appropriately.
 * Return: S1 and s2
 */
int compare_strs(char *s1, char *s2)
{
	int a = 0;

	while (s1[a] == s2[a] && s1[a] && s2[a])
	{
		a++;
	}

	return (s1[a] - s2[a]);
}
