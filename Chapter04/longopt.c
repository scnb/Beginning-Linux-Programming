#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE			//与头文件getopt.h一同包含进来，启用getopt_long功能
#include <getopt.h>

int main(int argc, char *argv[])
{
	int opt;
	struct option longopts[]=			//结构数组，描述每个长选项并告诉getopt_long()怎么处理它们
	{
		{"initialize",0,NULL,'i'},		//依次为长选项的名字、该选项是否带参数、NULL表示找到该选项时，getopt_long返回第四个成员的值、该选项要返回的值
		{"file",1,NULL,'f'},
		{"list",0,NULL,'l'},
		{"restart",0,NULL,'r'},
		{0,0,0,0}						//该结构体必须以此结尾
	};

	while ((opt = getopt_long(argc, argv, ":if:lr", longopts, NULL)) != -1)	//opt指向下一个选项字符
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
