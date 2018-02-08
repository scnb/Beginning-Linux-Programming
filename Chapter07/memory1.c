#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* 1MB大小 */
/* 1024B = 1KB, 1024个1KB是1MB */
#define A_MEGABYTE (1024 * 1024)

int main()
{
	char *some_memory;
	int megabyte = A_MEGABYTE;
	int exit_code = EXIT_FAILURE;

	/* 使用malloc函数分配空间，并使用强制转换将void *型指针转换为char *型指针 */
	some_memory = (char *)malloc(megabyte);
	if (some_memory != NULL)
	{
		sprintf(some_memory, "Hello World\n");
		printf("%s", some_memory);
		exit_code = EXIT_SUCCESS;
	}
	exit (exit_code);
}
