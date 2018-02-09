#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <gdbm-ndbm.h>
#include <string.h>

#define TEST_DB_FILE "/tmp/dbm2_test"
#define ITEMS_USED	(3)

struct test_data
{
	char misc_chars[15];
	int any_integer;
	char more_chars[21];
};

int main()
{
	struct test_data items_to_store[ITEMS_USED];
	struct test_data item_retrieved;

	/* 定义关键字字符串 */
	char key_to_use[20];
	int i, result;

	/* 定义datum结构，分别表示关键字（索引）和数据内容 */ 
	datum key_datum;
	datum data_datum;

	DBM *dbm_ptr;

	/* 打开测试数据库用来读写 */
	dbm_ptr = dbm_open(TEST_DB_FILE, O_RDWR | O_CREAT, 0666);
	if (!dbm_ptr)
	{
		fprintf(stderr, "Failed to open database\n");
		exit (EXIT_FAILURE);
	}

	/* 添加一些数据到items_to_store结构中 */
	memset(items_to_store, '\0', sizeof(items_to_store));
	strcpy(items_to_store[0].misc_chars, "First!");
	items_to_store[0].any_integer = 47;
	strcpy(items_to_store[0].more_chars, "foo");

	strcpy(items_to_store[1].misc_chars, "bar");
	items_to_store[1].any_integer = 13;
	strcpy(items_to_store[1].more_chars, "unlucky?");

	strcpy(items_to_store[2].misc_chars, "Third");
	items_to_store[2].any_integer = 3;
	strcpy(items_to_store[2].more_chars, "baz");

	/* 为每个数据项建立一个供以后引用的关键字（索引）*/
	for (i = 0;i < ITEMS_USED;i++)
	{
		sprintf(key_to_use, "%c%c%d", items_to_store[i].misc_chars[0], items_to_store[i].more_chars[0], items_to_store[i].any_integer);
		key_datum.dptr = (void *)key_to_use;
		key_datum.dsize = strlen(key_to_use);
		data_datum.dptr = (void *)&items_to_store[i];
		data_datum.dsize = sizeof(struct test_data);

		result = dbm_store(dbm_ptr, key_datum, data_datum, DBM_REPLACE);
		if (result != 0)
		{
			fprintf(stderr, "dbm_store failed on key %s\n", key_to_use);
			exit (2);
		}
	}

	/* 尝试删除数据项 */
	sprintf(key_to_use, "bu%d", 13);
	key_datum.dptr = key_to_use;
	key_datum.dsize = strlen(key_to_use);

	if (dbm_delete(dbm_ptr, key_datum) == 0)
	{
		printf("Data with key %s delected\n", key_to_use);
	}
	else
	{
		printf("Nothing deleted for key %s\n", key_to_use);
	}

	/* 对整个数据库进行循环，并输出其中存储的数据项 */
	for (key_datum = dbm_firstkey(dbm_ptr);key_datum.dptr;key_datum = dbm_nextkey(dbm_ptr))
	{
		/* 在dbm_ptr指向的数据库中查找key_datum索引指向的数据项 */
		data_datum = dbm_fetch(dbm_ptr, key_datum);
		if (data_datum.dptr)
		{
			printf("Data retrieved\n");
			memcpy(&item_retrieved, data_datum.dptr, data_datum.dsize);
			printf("Retrieved item - %s %d %s\n", item_retrieved.misc_chars, item_retrieved.any_integer, item_retrieved.more_chars);
		}
		else
		{
			printf("No data found for key %s\n", key_to_use);
		}
	}
	dbm_close(dbm_ptr);
	exit (EXIT_SUCCESS);
}
