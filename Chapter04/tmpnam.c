#include <stdio.h>
#include <stdlib.h>

int main()
{
	char tmpname[L_tmpnam];
	char *filename;
	FILE *tmpfp;

	filename = tmpnam(tmpname);	//tmpnam()函数为临时文件生成一个唯一的文件名，若tmpname不为空，则文件名也会写入tmpname，即tmpname中的内容与filename中的内容相同。

	printf("Temporary file name is: %s\n", filename);
	tmpfp = tmpfile();			//tmpfile()函数是一种更安全的做法，它创建并打开一个临时文件，且具有唯一的文件名，避免了另一个程序用同样的名字打开临时文件的风险。
	if (tmpfp)
	{
		printf("Opened a temporary file OK\n");
	}
	else
	{
		perror("tmpfile");
	}
	exit (0);
}
