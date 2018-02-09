/* 该程序用来实现用户接口，可以用其来访问数据库 */

#define _XOPEN_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "cd_data.h"

/* 下面的数值一定要比数据库中的任何数据项的长度都要大 */
#define TMP_STRING_LEN 125

/* 用typedef语句来定义菜单选项 */
typedef enum 
{
	mo_invalid,
	mo_add_cat,
	mo_add_tracks,
	mo_del_cat,
	mo_find_cat,
	mo_list_cat_tracks,
	mo_del_tracks,
	mo_count_entries,
	mo_exit
} menu_options;


static int command_mode(int argc, char *argv[]);
static void announce(void);
static menu_options show_menu(const cdc_entry *current_cdc);
static int get_comfirm(const char *question);
static int enter_new_cat_entry(cdc_entry *entry_to_delete);
static void enter_new_track_entries(const cdc_entry *entry_to_add_to);
static void del_cat_entry(const cdc_entry *entry_to_delete);
static void del_track_entries(const cdc_entry *entry_to_delete);
static cdc_entry find_cat(void);
static void list_tracks(const cdc_entry *entry_to_use);
static void count_all_entries(void);
static void display_cdc(const cdc_entry *cdc_to_show);
static void display_cdt(const cdt_entry *cdt_to_show);
static void strip_return(char *string_to_strip);


void main(int argc, char *argv[])
{
	/* 先对current_cdc_entry结构进行初始化，保存当前选中的CD标题项 */
	menu_options current_option;
	cdc_entry current_cdc_entry;
	int command_result;

	memset(&current_cdc_entry, '\0', sizeof(current_cdc_entry));
	
	/* 解析命令行，宣布正在运行的是哪个程序，并初始化数据库 */
	if (argc > 1)
	{
		command_result = command_mode(argc, argv);
		exit (command_result);
	}

	announce();

	if (!database_initialize(0))
	{
		fprintf(stderr, "Sorry, unable to initialize database\n");
		fprintf(stderr, "To create a new database use %s -i\n", argv[0]);
		exit (EXIT_FAILURE);
	}

	/* 进入循环，等待用户选择一个菜单选项，做相应处理，直到用户选择退出 */
	while (current_option != mo_exit)
	{
		/* 将current_cdc_entry作为参数传递，是为了使菜单能够根据用户当前选择的CD做相应改变 */
		current_option = show_menu(&current_cdc_entry);
		
		switch(current_option)
		{
			case mo_add_cat:
				if (enter_new_cat_entry(&current_cdc_entry))
				{
					if (!add_cdc_entry(current_cdc_entry))
					{
						fprintf(stderr, "Failed to add new entry\n");
						memset(&current_cdc_entry, '\0', sizeof(current_cdc_entry));
					}
				}
				break;
			case mo_add_tracks:
				enter_new_track_entries(&current_cdc_entry);
				break;
			case mo_del_cat:
				del_cat_entry(&current_cdc_entry);
				/* 很激动，我修复了原书上的一个BUG，即删除目录后，选择6列出曲目内容还能显示刚才的目录内容
				   修复办法：添加下面这句话，即在删除目录后，将current_cdc_entry清空，则后面显示屏幕时会
				   			 回到精简模式
				*/
				memset(&current_cdc_entry, '\0', sizeof(current_cdc_entry));
				break;
			case mo_find_cat:
				current_cdc_entry = find_cat();
				break;
			case mo_list_cat_tracks:
				list_tracks(&current_cdc_entry);
				break;
			case mo_del_tracks:
				del_track_entries(&current_cdc_entry);
				break;
			case mo_count_entries:
				count_all_entries();
			case mo_exit:
				break;
			default:
				break;
		}
	}

	/* 关闭数据库并退出 */
	database_close();
	exit (EXIT_SUCCESS);
}


/* 输出欢迎词 */
static void announce(void)
{
	printf("\n\nWelcome to the demonstration CD catalog database program\n");
}


/* 通过标题名的第一个字符来检查当前标题项是否被选中 */
static menu_options show_menu(const cdc_entry *cdc_selected)
{
	char tmp_str[TMP_STRING_LEN + 1];
	menu_options option_chosen = mo_invalid;

	while (option_chosen == mo_invalid)
	{
		/* 若当前选中了某个CD，则执行if语句的内容 */
		if (cdc_selected->catalog[0])
		{
			printf("\nCurrent entry: ");
			printf("%s, %s, %s, %s\n", cdc_selected->catalog, cdc_selected->title, cdc_selected->type, cdc_selected->artist);
			printf("\n");
			printf("1 - add new CD\n");
			printf("2 - search for a CD\n");
			printf("3 - cout the CDs and tracks in the database\n");
			printf("4 - re-enter tracks for current CD\n");
			printf("5 - delete this CD, and all its tracks\n");
			printf("6 - list tracks for this CD\n");
			printf("q - quit\n");
			printf("\nOption: ");
			/* 从标准输入流中读取字符串存在tmp_str中 */
			fgets(tmp_str, TMP_STRING_LEN, stdin);
			
			switch(tmp_str[0])
			{
				case '1': option_chosen = mo_add_cat;break;
				case '2': option_chosen = mo_find_cat;break;
				case '3': option_chosen = mo_count_entries;break;
				case '4': option_chosen = mo_add_tracks;break;
				case '5': option_chosen = mo_del_cat;break;
				case '6': option_chosen = mo_list_cat_tracks;break;
				case 'q': option_chosen = mo_exit;break;
			}
		}
		/* 若当前没有选中CD，则执行else语句的内容 */
		else
		{
			printf("\n\n");
			printf("1 - add new CD\n");
			printf("2 - search for a CD\n");
			printf("3 - count the CDs and tracks in the database\n");
			printf("q - quit\n");
			printf("\nOption: ");
			fgets(tmp_str, TMP_STRING_LEN, stdin);
			switch(tmp_str[0])
			{
				case '1': option_chosen = mo_add_cat;break;
				case '2': option_chosen = mo_find_cat;break;
				case '3': option_chosen = mo_count_entries;break;
				case 'q': option_chosen = mo_exit;break;
			}
		}
	}
	return (option_chosen);
}


/* 请求用户对其操作进行确认 */
static int get_confirm(const char *question)
{
	char tmp_str[TMP_STRING_LEN + 1];
	printf("%s", question);
	fgets(tmp_str, TMP_STRING_LEN, stdin);
	if (tmp_str[0] == 'Y' || tmp_str[0] == 'y')
	{
		return (1);
	}

	return 0;
}


/* 向数据库中添加新的标题项 */
static int enter_new_cat_entry(cdc_entry *entry_to_update)
{
	cdc_entry new_entry;
	char tmp_str[TMP_STRING_LEN + 1];

	memset(&new_entry, '\0', sizeof(new_entry));

	/* 依次输入并保存目录名、标题名、类型、艺术家 */
	printf("Enter catalog entry: ");
	(void)fgets(tmp_str, TMP_STRING_LEN, stdin);	
	/* 因为fgets函数得到的字符串包含换行符，所以，要将换行符去掉 */
	strip_return(tmp_str);
	strncpy(new_entry.catalog, tmp_str, CAT_CAT_LEN - 1);

	printf("Enter title: ");
	(void)fgets(tmp_str, TMP_STRING_LEN, stdin);
	strip_return(tmp_str);
	strncpy(new_entry.title, tmp_str, CAT_TITLE_LEN - 1);

	printf("Enter type: ");
	(void)fgets(tmp_str, TMP_STRING_LEN, stdin);
	strip_return(tmp_str);
	strncpy(new_entry.type, tmp_str, CAT_TYPE_LEN - 1);

	printf("Enter artist: ");
	(void)fgets(tmp_str, TMP_STRING_LEN, stdin);
	strip_return(tmp_str);
	strncpy(new_entry.artist, tmp_str, CAT_ARTIST_LEN - 1);

	printf("\nNew catalog entry entry is :-\n");
	display_cdc(&new_entry);
	if (get_confirm("Add this entry ?"))
	{
		memcpy(entry_to_update, &new_entry, sizeof(new_entry));
		return (1);
	}
	return (0);
}


/* 向数据库中添加曲目信息的函数，并且允许保留已经存在的曲目项 */
static void enter_new_track_entries(const cdc_entry *entry_to_add_to)
{
	cdt_entry new_track, existing_track;
	char tmp_str[TMP_STRING_LEN + 1];
	/* 将当前曲目编号设定为1 */
	int track_no = 1;
	if (entry_to_add_to->catalog[0] == '\0')
	{
		return;
	}
	printf("\nUpdating tracks for %s\n", entry_to_add_to->catalog);
	printf("Press return to leave existing description unchanged,\n");
	printf(" a single d to delete this and remaining tracks, \n");
	printf(" or new track description\n");

	while (1)
	{
		/* 首先，必须检查当前曲目编号处是否已存在曲目 */
		memset(&new_track, '\0', sizeof(new_track));
		existing_track = get_cdt_entry(entry_to_add_to->catalog, track_no);
		if (existing_track.catalog[0])
		{
			printf("\tTrack %d: %s\n", track_no, existing_track.track_txt);
			printf("\tNew text: ");
		}
		else
		{	
			printf("\tTrack %d description: ", track_no);
		}
		fgets(tmp_str, TMP_STRING_LEN, stdin);
		strip_return(tmp_str);

		/* 如果当前曲目编号处没有现存曲目，且用户也没有添加一条记录，则程序认为曲目已经添加完毕 */
		if (strlen(tmp_str) == 0)
		{
			/* 若用户没有添加记录 */
			if (existing_track.catalog[0] == '\0')
			{
				/* 若当前曲目编号处也没有现存曲目 */
				break;
			}
			else
			{
				/* 保持现存曲目不变，跳过 */
				track_no++;
				continue;
			}
		}

		/* 若用户输入一个字符d，则删除当前以及更高编号的曲目 */
		if ((strlen(tmp_str) == 1) && tmp_str[0] == 'd')
		{
			/* 通过循环来一直删除更高编号的曲目 */
			while (del_cdt_entry(entry_to_add_to->catalog, track_no))
			{
				/* track_no即曲目编号 */
				track_no++;
			}
			break;
		}

		/* 添加一个新的曲目或者更新一个现有曲目 */
		strncpy(new_track.track_txt, tmp_str, TRACK_TTEXT_LEN - 1);
		strcpy(new_track.catalog, entry_to_add_to->catalog);
		new_track.track_no = track_no;
		if (!add_cdt_entry(new_track))
		{
			fprintf(stderr, "Failed to add new track\n");
			break;
		}
		/* 处理下一个当前目录中的下一个曲目 */
		track_no++;
	}
}


/* 删除一个标题项，并且原来属于它的所有曲目也都被删除 */
static void del_cat_entry(const cdc_entry *entry_to_delete)
{
	int track_no = 1;
	int delete_ok;

	display_cdc(entry_to_delete);
	if (get_confirm("Delete this entry and all it's tracks? "))
	{
		do
		{
			delete_ok = del_cdt_entry(entry_to_delete->catalog, track_no);
			track_no++;
		} while(delete_ok);
		
		if (!del_cdc_entry(entry_to_delete->catalog))
		{
			fprintf(stderr, "Failed to delete entry\n");
		}
	}
}


/* 删除与某个标题对应的所有曲目，但是保留该目录 */
static void del_track_entries(const cdc_entry *entry_to_delete)
{
	int track_no = 1;
	int delete_ok;

	display_cdc(entry_to_delete);

	if (get_confirm("Delete tracks for this entry? "))
	{
		do
		{
			delete_ok = del_cdt_entry(entry_to_delete->catalog, track_no);
			track_no++;
		} while(delete_ok);
	}		
}


/* 用户输入一个字符串，查找包含该字符串的标题项 */
static cdc_entry find_cat(void)
{
	cdc_entry item_found;
	char tmp_str[TMP_STRING_LEN + 1];
	int first_call = 1;
	int any_entry_found = 0;
	int string_ok;
	int entry_selected = 0;

	/* 下面这个do-while循环的作用就是从用户那里得到合法的搜寻字符串 */
	do
	{
		string_ok = 1;
		printf("Enter string to search for in catalog entry: ");
		fgets(tmp_str, TMP_STRING_LEN, stdin);
		strip_return(tmp_str);
		if (strlen(tmp_str) > CAT_CAT_LEN)
		{
			fprintf(stderr, "Sorry, string too long, maxium %d characters\n", CAT_CAT_LEN);
			string_ok = 0;
		}
	} while (!string_ok);

	while (!entry_selected)
	{
		item_found = search_cdc_entry(tmp_str, &first_call);
		if (item_found.catalog[0] != '\0')
		{
			any_entry_found = 1;
			printf("\n");
			display_cdc(&item_found);
			/* 当用户确认后，将退出while循环 */
			if (get_confirm("This entry? "))
			{
				entry_selected = 1;
			}
		}
		else
		{
			if (any_entry_found)
			{
				printf("Sorry, no more matches found\n");
			}
			else
			{
				printf("Sorry, nothing found\n");
			}	
			break;
		}
	}
	return (item_found);
}

/* 输出指定标题项的所有曲目 */
static void list_tracks(const cdc_entry *entry_to_use)
{
	int track_no = 1;
	cdt_entry entry_found;

	/* 显示该CD的标题等信息 */
	display_cdc(entry_to_use);

	printf("\nTracks\n");
	do
	{
		entry_found = get_cdt_entry(entry_to_use->catalog, track_no);
		if (entry_found.catalog[0])
		{
			/* 显示该曲目的信息 */
			display_cdt(&entry_found);
			track_no++;
		}
	} while(entry_found.catalog[0]);
	(void)get_confirm("Press return");
}

/* 统计所有曲目的数量 */
static void count_all_entries(void)
{
	int cd_entries_found = 0;
	int track_entries_found = 0;
	cdc_entry cdc_found;
	cdt_entry cdt_found;
	int track_no = 1;
	int first_time = 1;
	char *search_string = "";

	/* 外层do-while循环对目录进行循环 */
	do
	{
		cdc_found = search_cdc_entry(search_string, &first_time);
		if (cdc_found.catalog[0])
		{
			cd_entries_found++;
			track_no = 1;
			/* 内层do-while循环对该目录中的曲目进行循环 */
			do
			{
				cdt_found = get_cdt_entry(cdc_found.catalog, track_no);
				if (cdt_found.catalog[0])
				{
					track_entries_found++;
					track_no++;
				}
			} while(cdt_found.catalog[0]);
		}
	} while(cdc_found.catalog[0]);

	printf("Found %d CDs, with a total of %d tracks\n", cd_entries_found, track_entries_found);
	(void)get_confirm("Press return");
}


/* 显示一条标题项记录 */
static void display_cdc(const cdc_entry *cdc_to_show)
{
	printf("Catalog: %s\n", cdc_to_show->catalog);
	printf("\tTitle: %s\n", cdc_to_show->title);
	printf("\tType: %s\n",  cdc_to_show->type);
	printf("\tArtist: %s\n",cdc_to_show->artist);
}


/* 显示一条曲目项记录 */
static void display_cdt(const cdt_entry *cdt_to_show)
{
	printf("%d: %s\n", cdt_to_show->track_no, cdt_to_show->track_txt);
}


/* 删除通过fgets函数得到的字符串尾部的换行符 */
static void strip_return(char *string_to_strip)
{
	int len;
	len = strlen(string_to_strip);
	if (string_to_strip[len - 1] == '\n')
	{
		string_to_strip[len - 1] = '\0';
	}
}


/* 对命令行参数进行解析 */
static int command_mode(int argc, char *argv[])
{
	int c;
	int result= EXIT_SUCCESS;
	char *prog_name = argv[0];

	/* 调用getopt函数确保程序能够接受符合标准Linux规范的参数 */

	while ((c = getopt(argc, argv, ":i")) != -1)
	{
		switch(c)
		{
			case 'i':
				if (!database_initialize(1))
				{
					result = EXIT_FAILURE;
					fprintf(stderr, "Failed to initialize database\n");
				}
				break;
			case ':':
			case '?':
			default:
				fprintf(stderr, "Usage: %s [-i]\n", prog_name);
				result = EXIT_FAILURE;
				break;
		}
	}
	return (result);
}


