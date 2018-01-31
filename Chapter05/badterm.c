#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>

int main()
{
	setupterm("unlisted", fileno(stdout), (int *)0);
	printf("Done.\n");
	exit (0);
}
