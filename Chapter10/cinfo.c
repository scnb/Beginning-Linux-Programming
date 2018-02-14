#include <stdio.h>
#include <stdlib.h>

/* 需要在编译该程序时，通过添加 -DDEBUG来启用调试  */
/* cc -o cinfo -DDEBUG cinfo.c */

int main()
{
	#ifdef DEBUG
		printf("Compiled: "__DATE__" at "__TIME__"\n");
		printf("This is line %d of file %s\n", __LINE__,__FILE__);
	#endif
	printf("hello world\n");
	exit (0);
}
