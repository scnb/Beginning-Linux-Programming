#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* 该程序不断分配1MB的内存块，并写入字符串，并记录分配了多少GB */

#define ONE_M	(1024 * 1024)

int main()
{
	char *some_memory;
	int size_to_allocate = ONE_M;
	int gegas_obtained = 0;
	int ms_obtained = 0;

	while (1)
	{
		for (ms_obtained = 0; ms_obtained < 1024; ms_obtained++)
		{
			some_memory = (char *)malloc(size_to_allocate);
			if (some_memory == NULL)
			{
				exit (EXIT_FAILURE);
			}
			else
			{
				sprintf(some_memory, "Hello, World");
			}
		}
		gegas_obtained++;
		printf("Now allocated %d gegasbytes\n", gegas_obtained);
	}
	exit (EXIT_SUCCESS);
}
