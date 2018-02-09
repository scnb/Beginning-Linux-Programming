/* 该头文件用于 定义数据的结构和用于访问这些结构的函数 */

/* The catalog table */
#define CAT_CAT_LEN		(30)
#define CAT_TITLE_LEN	(70)
#define CAT_TYPE_LEN	(30)
#define CAT_ARTIST_LEN	(70)

typedef struct
{
	char catalog[CAT_CAT_LEN + 1];
	char title[CAT_TITLE_LEN + 1];
	char type[CAT_TYPE_LEN + 1];
	char artist[CAT_ARTIST_LEN + 1];
} cdc_entry;


/* The tracks table */
#define TRACK_CAT_LEN	CAT_CAT_LEN
#define TRACK_TTEXT_LEN	(70)

typedef struct 
{
	char catalog[TRACK_CAT_LEN + 1];
	int track_no;	//该曲目的编号
	char track_txt[TRACK_TTEXT_LEN + 1];
} cdt_entry;

/* 下面介绍用于访问数据结构的函数 */

/* 初始化和终止函数 */
int database_initialize(const int new_database);
void database_close(void);

/* 两个取数据函数 */
cdc_entry get_cdc_entry(const char *cd_catalog_ptr);
cdt_entry get_cdt_entry(const char *cd_catalog_ptr, const int track_no);

/* 两个增加数据的函数 */
int add_cdc_entry(const cdc_entry entry_to_add);
int add_cdt_entry(const cdt_entry entry_to_add);

/* 两个删除数据的函数 */
int del_cdc_entry(const char *cd_catalog_ptr);
int del_cdt_entry(const char *cd_catalog_ptr, const int track_no);

/* （根据标题）检索函数 */
cdc_entry search_cdc_entry(const char *cd_catalog_ptr, int *first_call_ptr);
