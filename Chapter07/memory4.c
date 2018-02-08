#include <stdlib.h>

#define ONE_K	(1024)

int main()
{
	char *some_memory;
	char *scan_ptr;

	some_memory = (char *)malloc(ONE_K);
	if (some_memory == NULL)
	{
		exit (EXIT_FAILURE);
	}

	scan_ptr = some_memory;
	/* while循环中scan_ptr的自增最终会超过1KB的内存 */
	while (1)
	{
		scan_ptr = '\0';
		scan_ptr++;
	}
	exit (EXIT_SUCCESS);
}
