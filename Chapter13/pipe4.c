#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* 该程序是数据消费者，负责读取数据 */

int main(int argc, char *argv[])
{
	int data_processed;
	char buffer[BUFSIZ + 1];
	int file_descriptor;

	memset(buffer, '\0', sizeof(buffer));
	/* 获得作为参数传进来的文件描述符 */
	sscanf(argv[1], "%d", &file_descriptor);
	/* 从文件描述符处读取数据 */
	data_processed = read(file_descriptor, buffer, BUFSIZ);

	printf("%d - read %d bytes: %s\n", getpid(), data_processed, buffer);
	exit (EXIT_SUCCESS);
}
