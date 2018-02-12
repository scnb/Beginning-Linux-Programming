#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"

MYSQL my_connection;
MYSQL_RES *res_ptr;
MYSQL_ROW sqlrow;

void display_row(void);

int main(int argc, char *argv[])
{
	int res;

	mysql_init(&my_connection);
	/* 与数据库进行连接 */
	if (mysql_real_connect(&my_connection, "localhost", "rick", "secret", "foo", 0, NULL, 0))
	{
		printf("Connection success\n");
		/* 执行SQL语句 */
		res = mysql_query(&my_connection, "SELECT childno, fname, age FROM children WHERE age > 5");

		if (res)
		{
			printf("SELECT error: %s\n", mysql_error(&my_connection));
		}
		else
		{
			/* 将数据库中的数据一次性全部取出，该函数返回一个指向结果集结构的指针 */
			res_ptr = mysql_use_result(&my_connection);
			if (res_ptr)
			{
				/* 当使用mysql_use_result来获取数据时，将无法使用mysql_num_rows语句得到数据库中记录的数目 */
				/* 一次从结果集中提取一行 */
				while ((sqlrow = mysql_fetch_row(res_ptr)))
				{
					printf("Fetched data……\n");
					display_row();
				}

				if (mysql_errno(&my_connection))
				{
					fprintf(stderr, "Retrieved error: %s\n", mysql_error(&my_connection));
				}
				/* 释放查询占用的内存资源，进行清理 */
				mysql_free_result(res_ptr);
			}	
		}
		mysql_close(&my_connection);
	}
	else
	{
		fprintf(stderr, "Connection failed\n");
		if (mysql_errno(&my_connection))
		{
			fprintf(stderr, "Connection error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
		}
	}
	return EXIT_SUCCESS;
}


void display_row()
{
	unsigned int field_count;

	field_count = 0;
	while (field_count < mysql_field_count(&my_connection))
	{
		printf("%s ", sqlrow[field_count]);
		field_count++;
	}
	printf("\n");
}
