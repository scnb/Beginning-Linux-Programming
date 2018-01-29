#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *var, *value;

	if (argc == 1 || argc > 3)
	{
		fprintf(stderr, "usage: environ var [value]\n");
		exit (1);
	}	
	/* 调用getenv从环境中取出变量的值 */
	var = argv[1];	//取得第一个命令行参数
	value = getenv(var);	//取得该参数的值
	if (value)				//若值存在，则执行if语句块
	{
		printf("Variable %s has value %s\n", var, value);
	}
	else					//若该参数（变量）不存在，或者变量存在，但是没有关联的值，则返回null
	{
		printf("Variable %s has no value\n", var);
	}
	
	/* 检查程序是否还有第二个参数，若有，则构建一个格式为"名字=值"的字符串并调用putenv来设置变量的值 */
	if (argc == 3)		//argc==3，说明一共有两个参数
	{
		char *string;
		value = argv[2];
		string = malloc(strlen(var) + strlen(value) + 2);		//分配能够存储变量、变量值、及等号的内存空间
		if (!string)
		{
			fprintf(stderr,"out of memory\n");
			exit (1);
		}
		strcpy(string,var);
		strcat(string,"=");
		strcat(string,value);
		printf("Calling putenv with: %s\n",string);
		if (putenv(string) != 0)
		{
			fprintf(stderr, "putenv failed\n");
			free(string);
			exit (1);
		}
		/* 再次调用getenv来查看变量的新值 */
		value = getenv(var);
		if (value)
		{
			printf("New value of %s is %s\n", var, value);
		}
		else
		{
			printf("New value of %s is null??\n", var);
		}	
	}
	exit (0);
}
