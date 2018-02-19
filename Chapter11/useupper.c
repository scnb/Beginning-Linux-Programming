#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char *filename;
	if (argc != 2)
	{
		fprintf(stderr, "usage: useupper file\n");
		exit (1);
	}
	filename = argv[1];

	/* 用freopen函数先关闭标准输入，然后将文件流stdin与程序参数给定的文件名关联起来 */
	if (!freopen(filename, "r", stdin))
	{
		fprintf(stderr, "could not redirect stdin from file %s\n", filename);
		exit (2);
	}

	execl("./upper", "upper", 0);
	perror("could not exec ./upper");
	exit (3);
}	
