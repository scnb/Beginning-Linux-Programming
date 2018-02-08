#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* Linux（在GNU C函数的包装下) */

int main()
{
	char *some_memory = (char *)0;
	/* 访问空指针 */
	printf("A read from null %s\n", some_memory);
	sprintf(some_memory, "A write to null\n");
	exit (EXIT_SUCCESS);
}
