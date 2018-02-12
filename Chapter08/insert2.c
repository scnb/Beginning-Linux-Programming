#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"

int main(int argc, char *argv[])
{
	/* 自定义一个连接句柄作为变量 */
	MYSQL my_connection;
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;

	int res;

	/* 使用非动态分配的连接结构 */
	mysql_init(&my_connection);

	if (mysql_real_connect(&my_connection, "localhost", "rick", "secret", "foo", 0, NULL, 0))
	{
		printf("Connection success\n");
		res = mysql_query(&my_connection, "INSERT INTO children(fname, age) VALUES('Robert', 7)");
		
		if (!res)
		{
			printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(&my_connection));
		}
		else
		{
			fprintf(stderr, "Insert error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
		}

		/* 在插入一行之后，使用 LAST_INSERT_ID()来获得分配的ID */
		res = mysql_query(&my_connection, "SELECT LAST_INSERT_ID()");

		if (res)
		{
			printf("SELECT error: %s\n", mysql_error(&my_connection));
		}
		else
		{
			/* 使用mysql_use_resutl()从执行的SELECT语句中获取数据并将它打印出来 */
			res_ptr = mysql_use_result(&my_connection);
			if (res_ptr)
			{
				while ((sqlrow = mysql_fetch_row(res_ptr)))
				{
					printf("We inserted childno %s\n", sqlrow[0]);
				}
				mysql_free_result(res_ptr);
			}
		}
		mysql_close(&my_connection);
	}
	else
	{
		fprintf(stderr, "Connectin failed\n");
		if (mysql_errno(&my_connection))
		{
			fprintf(stderr, "Connection  error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
		}
	}
	return EXIT_SUCCESS;
}	
