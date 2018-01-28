#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if ((dp = opendir(dir)) == NULL)
	{
		fprintf(stderr, "cannot open directory: %s\n", dir);
		return;
	}
	chdir(dir);							//调用dir切换到指定目录
	while ((entry = readdir(dp)) != NULL)
	{
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode))					//判断是不是目录，若是目录，则进入if语句
		{
			/*Found a directory, but ignore . and ..*/
			if (strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)	//检查该数据项是不是.或者..
			{
				continue;
			}
			printf("%*s%s\n",depth,"",entry->d_name);	//如果不是.或者..，则输出以depth缩排的目录名
			/* 递归遍历 */
			printdir(entry->d_name,depth+4);			//对该目录进行递归调用将该目录中的所有子目录和文件的名字都输出，同时缩进加4个空格
		}
		else
		{
			printf("%*s%s\n",depth,"",entry->d_name);	//如果不是目录，则输出文件的名字
		}
	}
	chdir("..");	//当while循环结束后，即当前目录遍历完后，退回到上一层目录，然后继续目录的遍历
	closedir(dp);	//调用closedir及时关闭遍历完的目录，确保了打开的目录流数目不超出限制
}

int main(int argc, char *argv[])
{
	char *topdir = ".";
	if (argc >= 2)
	{
		topdir = argv[1];
	}
	printf("Directory scan of %s:\n",topdir);
	printdir(topdir,0);
	printf("done.\n");
	exit (0);
}
