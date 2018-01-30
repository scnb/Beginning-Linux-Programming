#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *f;

	f = fopen("not here", "r");
	if (!f)
	{	
		syslog(LOG_ERR|LOG_USER,"oops - %m\n");
	}
	exit (0);
}
