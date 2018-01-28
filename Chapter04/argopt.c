#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int opt;

	while ((opt = getopt(argc, argv, ":if:lr")) != -1)	//opt指向下一个选项字符
	{
		switch(opt)
		{
			case 'i':
			case 'l':
			case 'r':
				printf("option: %c\n",opt);
				break;
			case 'f':
				printf("filename: %s\n",optarg);		//optarg是一个外部变量，指向选项的关联值
				break;
			case ':':
				printf("option needs a value\n");
				break;
			case '?':
				printf("unknown option: %c\n",optopt);	//optopt存储无法识别的选项
		}
	}
	for(;optind<argc;optind++)
	{
		printf("argument: %s\n",argv[optind]);			//从argv数组中optind位置处开始输出所有的非选项的参数
	}
	exit (0);
}
