#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* 在这个程序中，将请求比机器的实际物理内存更多的内存 */

#define A_MEGABYTE (1024 * 1024)
#define PHY_MEM_MEGS 1024

int main()
{
	char *some_memory;
	size_t size_to_allocate = A_MEGABYTE;
	int megs_obtained = 0;

	/* 当已分配的内存兆数小于实际物理内存的8倍时循环 */
	while (megs_obtained < (PHY_MEM_MEGS * 8))	
	{
		some_memory = (char *)malloc(size_to_allocate);
		if (some_memory != NULL)
		{
			/* 分配内存空间加1MB */
			megs_obtained++;
			sprintf(some_memory, "Hello World");
			printf("%s - now allocated %d Megabytes\n", some_memory, megs_obtained);
		}
		else
		{
			exit (EXIT_FAILURE);
		}
	}
	exit (EXIT_SUCCESS);
}
