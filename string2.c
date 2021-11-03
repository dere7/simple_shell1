#include <unistd.h>
#include <limits.h>
#include "main.h"

/**
 * print_num - prints an int
 * @n: int to print
 *
 * Return: number of printed character
 */
int print_num(long n)
{
	unsigned int absolute, aux, countnum, count;

	count = 0;
	if (n < 0)
	{
		absolute = (n * -1);
		count += _putchar('-');
	}
	else
		absolute = n;

	aux = absolute;
	countnum = 1;
	while (aux > 9)
	{
		aux /= 10;
		countnum *= 10;
	}
	while (countnum >= 1)
	{
		count += _putchar(((absolute / countnum) % 10) + '0');
		countnum /= 10;
	}
	return (count);
}
/**
 * print - print str to stdout
 * @s: string to be printed
 */
void print(char *s)
{
	write(STDOUT_FILENO, s, _strlen(s));
}

