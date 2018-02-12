#include <stdlib.h>
#include <stdio.h>

/* 可以根据make命令的内置规则（推导规则）来简化甚至不用编写makefile文件
   例如编译本程序可以使用如下命令：
   make foo
   或者通过在命令行上修改宏来改变其默认行为：
   make CC=gcc CFLAGS="-g -Wall" foo
*/

int main()
{
	printf("Hello, World\n");
	exit (EXIT_SUCCESS);
}
