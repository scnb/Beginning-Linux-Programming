/* 本程序含有访问dbm数据库的函数 */

#define _XOPEN_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include <gdbm-ndbm.h>
#include "cd_data.h"

#define CDC_FILE_BASE "cdc_data"
#define CDT_FILE_BASE "cdt_data"
#define CDC_FILE_DIR  "cdc_data.dir"
#define CDC_FILE_PAG  "cdc_data.pag"
#define CDT_FILE_DIR  "cdt_data.dir"
#define CDT_FILE_PAG  "cdt_data.pag"

/* 使用下面两个文件范围变量追踪当前的数据库 */
static DBM *cdc_dbm_ptr = NULL;
static DBM *cdt_dbm_ptr = NULL;


/* database_initialize函数默认打开一个已存在的数据库，
   但是，当传递的参数不为0时，可以强迫其创建一个新的
   (空的)数据库，并手动删除.dir和.pag文件。
*/
int database_initialize(const int new_database)
{
	int open_mode = O_CREAT | O_RDWR;

	/* 如果已存在的数据库被打开了，则关闭它 */
	if (cdc_dbm_ptr)
	{
		dbm_close(cdc_dbm_ptr);
	}
	if (cdt_dbm_ptr)
	{
		dbm_close(cdt_dbm_ptr);
	}

	if (new_database)
	{
		/* 删除旧的文件 */
		(void) unlink(CDC_FILE_PAG);
		(void) unlink(CDC_FILE_DIR);
		(void) unlink(CDT_FILE_PAG);
		(void) unlink(CDT_FILE_DIR);
	}

	/* 打开/创建一些新的文件 */
	cdc_dbm_ptr = dbm_open(CDC_FILE_BASE, open_mode, 0644);
	cdt_dbm_ptr = dbm_open(CDT_FILE_BASE, open_mode, 0644);
	if (!cdc_dbm_ptr || !cdt_dbm_ptr)
	{
		fprintf(stderr, "Unable to create database\n");
		cdc_dbm_ptr = cdt_dbm_ptr = NULL;
		return (0);
	}
	return (1);
}

/* 关闭已打开的数据库 */
void database_close(void)
{
	if (cdc_dbm_ptr)
	{
		dbm_close(cdc_dbm_ptr);
	}
	if (cdt_dbm_ptr)
	{
		dbm_close(cdt_dbm_ptr);
	}
	cdc_dbm_ptr = cdt_dbm_ptr = NULL;
}


/* 向该函数传递一个指向标题的指针，检索出一个标题来 */
cdc_entry get_cdc_entry(const char *cd_catalog_ptr)
{
	cdc_entry entry_to_return;
	char entry_to_find[CAT_CAT_LEN + 1];
	datum local_data_datum;
	datum local_key_datum;

	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	/* 做一些安全性检查，确保数据库已打开且用户传递了合适的参数 */
	/* 检查数据库是否打开 */
	if (!cdc_dbm_ptr || !cdt_dbm_ptr)
	{
		return (entry_to_return);
	}
	/* 检查是否传递了合适参数 */
	if (!cd_catalog_ptr)
	{
		return (entry_to_return);
	}
	if (strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (entry_to_return);
	}

	memset(&entry_to_find, '\0', sizeof(entry_to_find));
	strcpy(entry_to_find, cd_catalog_ptr);

	/* 设置datum结构，使用dbm_fetch函数来检索数据 */
	local_key_datum.dptr = (void *)entry_to_find;
	local_key_datum.dsize = sizeof(entry_to_find);

	memset(&local_data_datum, '\0', sizeof(local_data_datum));
	local_data_datum = dbm_fetch(cdc_dbm_ptr, local_key_datum);
	if (local_data_datum.dptr)
	{
		memcpy(&entry_to_return, (char *)local_data_datum.dptr, local_data_datum.dsize);
	}
	return (entry_to_return);
}


/* 通过一个指向标题字符串的指针和一个曲目编号对一个单独的曲目进行检索 */
cdt_entry get_cdt_entry(const char *cd_catalog_ptr, const int track_no)
{
	cdt_entry entry_to_return;
	char entry_to_find[CAT_CAT_LEN + 10];
	datum local_data_datum;
	datum local_key_datum;

	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	if (!cdc_dbm_ptr || !cdt_dbm_ptr)
	{
		return (entry_to_return);
	}
	if (!cd_catalog_ptr)
	{
		return (entry_to_return);
	}
	if (strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (entry_to_return);
	}

	/* 构建索引参数，由标题指针和曲目编号组成 */
	memset(&entry_to_find, '\0', sizeof(entry_to_find));
	sprintf(entry_to_find, "%s %d", cd_catalog_ptr, track_no);

	local_key_datum.dptr = (void *)entry_to_find;
	local_key_datum.dsize = sizeof(entry_to_find);

	memset(&local_data_datum, '\0', sizeof(local_data_datum));
	local_data_datum = dbm_fetch(cdt_dbm_ptr, local_key_datum);

	if (local_data_datum.dptr)
	{
		memcpy(&entry_to_return, (char *)local_data_datum.dptr, local_data_datum.dsize);
	}
	return (entry_to_return);
}


/* 增加一个新的标题项记录 */
int add_cdc_entry(const cdc_entry entry_to_add)
{
	char key_to_add[CAT_CAT_LEN + 1];
	datum local_data_datum;
	datum local_key_datum;
	int result;

	/* 检查数据库是否打开，并且传入的参数是否有效 */
	if (!cdc_dbm_ptr || !cdt_dbm_ptr)
	{
		return (0);
	}
	if (strlen(entry_to_add.catalog) >= CAT_CAT_LEN)
	{
		return (0);
	}

	memset(&key_to_add, '\0', sizeof(key_to_add));
	strcpy(key_to_add, entry_to_add.catalog);

	local_key_datum.dptr = (void *)key_to_add;
	local_key_datum.dsize = sizeof(key_to_add);
	local_data_datum.dptr = (void *)&entry_to_add;
	local_data_datum.dsize = sizeof(entry_to_add);

	result = dbm_store(cdc_dbm_ptr, local_key_datum, local_data_datum, DBM_REPLACE);

	/* dbm_store函数返回0表示成功 */
	if (result == 0)
	{
		return (1);
	}
	return (0);
}


/* 增加一个新的曲目项记录，将标题项和曲目项合在一起构成访问关键字 */
int add_cdt_entry(const cdt_entry entry_to_add)
{
	char key_to_add[CAT_CAT_LEN + 10];
	datum local_data_datum;
	datum local_key_datum;
	int result;

	if (!cdc_dbm_ptr || !cdt_dbm_ptr)
	{
		return (0);
	}
	if (strlen(entry_to_add.catalog) >= CAT_CAT_LEN)
	{
		return (0);
	}

	memset(&key_to_add, '\0', sizeof(key_to_add));
	sprintf(key_to_add, "%s %d", entry_to_add.catalog, entry_to_add.track_no);

	/* 填充datum数据结构 */
	local_key_datum.dptr = (void *)key_to_add;
	local_key_datum.dsize = sizeof(key_to_add);
	local_data_datum.dptr = (void *)&entry_to_add;
	local_data_datum.dsize = sizeof(entry_to_add);

	result = dbm_store(cdt_dbm_ptr, local_key_datum, local_data_datum, DBM_REPLACE);
	if (result == 0)
	{
		return (1);
	}
	return (0);
}


/* 删除标题项记录 */
int del_cdc_entry(const char *cd_catalog_ptr)
{
	char key_to_del[CAT_CAT_LEN + 1];
	datum local_key_datum;
	int result;

	if (!cdc_dbm_ptr || !cdt_dbm_ptr)
	{
		return (0);
	}
	if (strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (0);
	}

	memset(&key_to_del, '\0', sizeof(key_to_del));
	strcpy(key_to_del, cd_catalog_ptr);

	local_key_datum.dptr = (void *)key_to_del;
	local_key_datum.dsize = sizeof(key_to_del);

	result = dbm_delete(cdc_dbm_ptr, local_key_datum);

	if (result == 0)
	{
		return (1);
	}
	return (0);
}


/* 删除曲目记录，曲目关键字是由标题项和曲目编号构成的符合索引 */
int del_cdt_entry(const char *cd_catalog_ptr, const int track_no)
{
	char key_to_del[CAT_CAT_LEN + 10];
	datum local_key_datum;
	int result;

	if (!cdc_dbm_ptr || !cdt_dbm_ptr)
	{
		return (0);
	}
	if (strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (0);
	}

	/* 构建曲目关键字 */
	memset(&key_to_del, '\0', sizeof(key_to_del));
	sprintf(key_to_del, "%s %d", cd_catalog_ptr, track_no);

	local_key_datum.dptr = (void *)key_to_del;
	local_key_datum.dsize = sizeof(key_to_del);

	/* 最关键的函数，根据数据库指针，和关键字索引来删除某一数据项 */
	result = dbm_delete(cdt_dbm_ptr, local_key_datum);

	if (result == 0)
	{
		return (1);
	}
	return (0);
}


/* 搜索函数，在预先不知道关键字的情况下扫描全部的dbm记录项 */
cdc_entry search_cdc_entry(const char *cd_catalog_ptr, int *first_call_ptr)
{
	static int local_first_call = 1;
	cdc_entry entry_to_return;
	datum local_data_datum;
	/* 注意该变量是static的 */
	static datum local_key_datum;
	
	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	/* 先做完整性检查 */
	if (!cdc_dbm_ptr || !cdt_dbm_ptr)
	{
		return (entry_to_return);
	}
	if (!cd_catalog_ptr || !first_call_ptr)
	{
		return (entry_to_return);
	}
	if (strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (entry_to_return);
	}

	/* …………某种功能…………*/
	if (local_first_call)
	{
		local_first_call = 0;
		*first_call_ptr = 1;
	}

	/* 如果*first_call_ptr为True，则从数据库开头搜索；
	   否则，移动到数据库中的下一个关键字
	*/
	if (*first_call_ptr)
	{
		*first_call_ptr = 0;	
		/* 之前将local_key_datum变量设为static的了，当重入该函数时，该变量能够记录之前搜索到数据库的哪个位置 */
		local_key_datum = dbm_firstkey(cdc_dbm_ptr);
	}
	else
	{
		local_key_datum = dbm_nextkey(cdc_dbm_ptr);
	}

	do
	{
		if (local_key_datum.dptr != NULL)
		{
			/* 根据索引从数据库中取出对应的数据项 */
			local_data_datum = dbm_fetch(cdc_dbm_ptr, local_key_datum);
			if (local_data_datum.dptr)
			{
				/* 将数据项中的内容复制到entry_to_return中 */
				memcpy(&entry_to_return, (char *)local_data_datum.dptr, local_data_datum.dsize);
				
				/* 检查当前标题项是否包含搜索字符串，如果包含则退出循环，否则继续循环 */
				if (!strstr(entry_to_return.catalog, cd_catalog_ptr))
				{
					/* 将entry_to_return赋值为\0后，则满足循环条件，则继续循环 */
					memset(&entry_to_return, '\0', sizeof(entry_to_return));
					/* 如果不包含的话，则取出下一个关键字在进行循环 */
					local_key_datum = dbm_nextkey(cdc_dbm_ptr);
				}
			}
		}
	} while(local_key_datum.dptr && local_data_datum.dptr && (entry_to_return.catalog[0] == '\0'));
	return (entry_to_return);
}
