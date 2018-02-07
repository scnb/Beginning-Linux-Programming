#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>

/* 定义一些全局常量 */
#define MAX_STRING 80		/* Longest allowed response */
#define MAX_ENTRY  1024	 	/* Longest allowed database entry */

#define MESSAGE_LINE	6	/* messages on this line */
#define ERROR_LINE		22	/* line to use for errors */
#define Q_LINE			20	/* line for questions */
#define PROMPT_LINE		18	/* line for prompting on */

/* 与更新数据库记录有关的一些全局常量 */
#define BOXED_LINES		11
#define BOXED_ROWS		60
#define BOX_LINE_POS 	8
#define BOX_ROW_POS		2

/* 定义一些全局变量 */
static char current_cd[MAX_STRING] = "\0";		/* 保存正在处理的当前CD唱片的标题 */
static char current_cat[MAX_STRING];			/* 记录当前CD唱片的分类号 */

/* 一些文件名的声明 */
const char *title_file = "title.cdb";
const char *tracks_file = "tracks.cdb";
const char *temp_file = "cdb.tmp";

/* 所有函数的原型声明 */
void clear_all_screen(void);
void get_return(void);
int get_confirm(void);
int getchoice(char *greet, char *choices[]);
void draw_menu(char *options[], int highlight, int start_row, int start_col);
void insert_title(char *cdtitle);
void get_string(char *string);
void add_record(void);
void count_cds(void);
void find_cd(void);
void list_tracks(void);
void remove_tracks(void);
void remove_cd(void);
void update_cd(void);


/* 定义一些菜单结构，即菜单选项数组，当某一个选项被选中时，返回选项的第一个字符 */
char *main_menu[] = {
	"add new CD",
	"find CD",
	"count CDs and tracks in the catalog",
	"quit",
	0,
};

char *extended_menu[] = {
	"add new CD",
	"find CD",
	"count CDs and tracks in the catalog",
	"list tracks on current CD",
	"remove current CD",
	"update track information",
	"quit", 
	0,
};

/* 用户从菜单中进行选择，直到用户选中quit退出为止 */
int main()
{
	int choice;
	initscr();
	do 
	{
		/* 当current_cd[0]不为空时，表明当前已选中cd，则显示扩展的菜单，某则显示精简的菜单 */
		choice = getchoice("Options:", current_cd[0] ? extended_menu : main_menu);
		switch(choice)
		{		
			case 'q':break;
			case 'a':add_record();break;
			case 'c':count_cds();break;
			case 'f':find_cd();break;
			case 'l':list_tracks();break;
			case 'r':remove_cd();break;
			case 'u':update_cd();break;
		}
	} while (choice != 'q');
	
	endwin();
	exit (EXIT_SUCCESS);
}

/* 											建立菜单												*/

int getchoice(char *greet, char *choices[])
{
	static int selected_row = 0;			//当前选中的行
	int max_row = 0;
	int start_screenrow = MESSAGE_LINE, start_screencol = 10;
	char **option;
	int selected;							//是否被选中的标志位
	int key = 0;

	option = choices;
	while (*option)
	{
		max_row++;
		option++;
	}

	/* 如果当前选中的行号比最大的行号还大，则将行号清零0，就像回滚一样 */
	if (selected_row >= max_row)
	{
		selected_row = 0;
	}
	clear_all_screen();
	mvprintw(start_screenrow -2, start_screencol, greet);
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	key = 0;
	/* 当key为以下3种之一时，则表示用户选择退出或者选择确认 */
	while (key != 'q' && key != KEY_ENTER && key != '\n')
	{
		/* 下面两个if语句是很简单的根据用户按键是上键或下键来修改当前选中的行selected_row */
		if (key == KEY_UP)
		{
			if (selected_row == 0)
			{
				selected_row = max_row -1;
			}
			else
			{
				selected_row--;
			}
		}
		if (key == KEY_DOWN)
		{		
			if (selected_row == (max_row -1 ))
			{
				selected_row = 0;
			}
			else
			{
				selected_row++;
			}
		}
		selected = *choices[selected_row];		//根据选中的行号来确定选项
		draw_menu(choices, selected_row, start_screenrow, start_screencol);
		key = getch();	//获取按键信息	
	}
	keypad(stdscr, FALSE);
	nocbreak();
	echo();

	if (key == 'q')
	{
		selected = 'q';
	}

	return (selected);
}


/* 显示当前菜单，用高亮来表示当前选中的是哪一行 */
void draw_menu(char *options[], int current_highlight, int start_row, int start_col)
{
	int current_row = 0;
	char **option_ptr;
	char *txt_ptr;
	option_ptr = options;
	while (*option_ptr)
	{
		if (current_row == current_highlight)
		{
			attron(A_STANDOUT);
		}
		txt_ptr = options[current_row];
		txt_ptr++;
		mvprintw(start_row + current_row, start_col, "%s", txt_ptr);
		if (current_row == current_highlight)
		{
			attroff(A_STANDOUT);
		}
		current_row++;
		option_ptr++;
	}
	mvprintw(start_row + start_row + 3, start_col, "Move highlight then press Return");
	refresh();
}

/* 清屏函数，并显示软件的名称，如果当前选中了CD，则显示其信息 */
void clear_all_screen()
{
	clear();
	mvprintw(2, 20, "%s", "CD Database Application");
	if (current_cd[0])
	{
		mvprintw(ERROR_LINE, 0, "Current CD: %s: %s\n", current_cat, current_cd);
	}
	refresh();
}


/*										操作数据库文件										*/

/* 添加一张新CD唱片的资料的到数据库 */
void add_record()
{
	char catalog_number[MAX_STRING];
	char cd_title[MAX_STRING];
	char cd_type[MAX_STRING];
	char cd_artist[MAX_STRING];
	char cd_entry[MAX_STRING];

	int screenrow = MESSAGE_LINE;
	int screencol = 10;

	clear_all_screen();
	mvprintw(screenrow, screencol, "Enter new CD details");
	screenrow += 2;

	mvprintw(screenrow, screencol, "Catalog Number: ");
	get_string(catalog_number);
	screenrow++;

	mvprintw(screenrow, screencol, "	 CD Title: ");
	get_string(cd_title);
	screenrow++;
	
	mvprintw(screenrow, screencol, "	  CD Type: ");
	get_string(cd_type);
	screenrow++;

	mvprintw(screenrow, screencol, "	CD Artist: ");
	get_string(cd_artist);
	screenrow++;

	mvprintw(15, 5, "About to add this new entry:");
	sprintf(cd_entry, "%s, %s, %s, %s", catalog_number, cd_title, cd_type, cd_artist);
	
	mvprintw(17, 5, "%s", cd_entry);
	refresh();
	move(PROMPT_LINE, 0);
	if (get_confirm)
	{
		insert_title(cd_entry);
		strcpy(current_cd, cd_title);
		strcpy(current_cat, catalog_number);
	}

}

/* 从屏幕当前位置读取一个字符串，并将其末尾可能存在的新行符删除 */
void get_string(char *string)
{
	int len;

	wgetnstr(stdscr, string, MAX_STRING);		//从当前屏幕获取最多MAX_STRING个字符
	len = strlen(string);
	if (len > 0 && string[len - 1] == '\n')
	{
		string[len - 1] = '\0';
	}
}


/* 提示并读取用户的确认信息 */
int get_confirm()
{
	int confirmed = 0;
	char first_char;
	mvprintw(Q_LINE, 5, "Are you sure?");
	clrtoeol();
	refresh();

	/* 调用cbreak()将输入模式设置为cbreak模式，在这种模式下，字符一经输入就被立刻传递给程序 */
	cbreak();
	first_char = getch();
	if (first_char == 'Y' || first_char == 'y')
	{
		confirmed = 1;
	}
	nocbreak();

	if (!confirmed)
	{
		mvprintw(Q_LINE, 1, "	Cancelled");
		clrtoeol();
		refresh();
		sleep(1);
	}
	return confirmed;
}


/* 将标题字符串添加到标题文件的末尾，从而在CD数据库中添加一个标题记录 */
void insert_title(char *cdtitle)
{
	FILE *fp = fopen(title_file, "a");		//a表示append，即追加
	if (!fp)
	{
		mvprintw(ERROR_LINE, 0, "cannot open CD titles database");
	}
	else
	{
		fprintf(fp, "%s\n", cdtitle);
		fclose(fp);
	}
}


/* 允许用户重新输入当前CD唱片中的曲目 */
void update_cd()
{
	FILE *tracks_fp;
	char track_name[MAX_STRING];
	int len;
	int track = 1;
	int screen_line = 1;
	WINDOW *box_window_ptr;
	WINDOW *sub_window_ptr;

	clear_all_screen();
	mvprintw(PROMPT_LINE, 0, "Re-entering tracks for CD.");
	if (!get_confirm())
	{
		return ;
	}
	move(PROMPT_LINE, 0);
	clrtoeol();

	remove_tracks();

	mvprintw(MESSAGE_LINE, 0, "Enter a blank line to finish");
	tracks_fp = fopen(tracks_file, "a");

	/* 先建立一个子窗口，围绕它画一个框，然后在这个带边框的子窗口内再添加一个新的卷屏子窗口 */
	box_window_ptr = subwin(stdscr, BOXED_LINES + 2, BOXED_ROWS + 2, BOX_LINE_POS - 1, BOX_ROW_POS - 1);
	if (!sub_window_ptr)
	{
		return;
	}
	box(box_window_ptr, ACS_VLINE, ACS_HLINE);		//在上面创建的子窗口周围画一个边框
	sub_window_ptr = subwin(stdscr, BOXED_LINES, BOXED_ROWS, BOX_LINE_POS, BOX_ROW_POS);
	if (!sub_window_ptr)
	{
		return;
	}
	scrollok(sub_window_ptr, TRUE);		//允许子窗口卷屏
	werase(sub_window_ptr);				//对子窗口清屏
	touchwin(stdscr);					//强制更新标准屏幕

	do
	{
		mvwprintw(sub_window_ptr, screen_line++, BOX_ROW_POS + 2, "Track %d: ", track);
		clrtoeol();
		refresh();
		wgetnstr(sub_window_ptr, track_name, MAX_STRING);
		len = strlen(track_name);
		if (len > 0 && track_name[len - 1] == '\n')
		{
			track_name[len -1 ] = '\0';
		}
		if (*track_name)
		{
			fprintf(tracks_fp, "%s,%d,%s\n", current_cat, track,track_name);
		}
		track++;
		if (screen_line > BOXED_LINES - 1)
		{
			scroll(sub_window_ptr);
			screen_line--;
		}
	} while (*track_name);
	delwin(sub_window_ptr);
	fclose(tracks_fp);
}


/* 删除database中的记录 */
void remove_cd()
{
	FILE *titles_fp, *temp_fp;
	char entry[MAX_ENTRY];
	int cat_length;

	if (current_cd[0] == '\0')	//判断当前是否选中了CD
	{
		return; 
	}

	clear_all_screen();
	mvprintw(PROMPT_LINE, 0, "About to remove CD %s: %s.", current_cat, current_cd);
	if (!get_confirm())
	{
		return ;
	}

	cat_length = strlen(current_cat);

	/* 将titles文件中的信息先转存到一个临时文件中 */
	titles_fp = fopen(title_file, "r");
	temp_fp = fopen(temp_file, "w");

	while (fgets(entry, MAX_ENTRY, titles_fp))
	{
		if (strncmp(current_cat, entry , cat_length) != 0)
		{
			fputs(entry, temp_fp);
		}
	}
	fclose(titles_fp);
	fclose(temp_fp);

	/* 删除titles文件，之后重命名临时文件 */
	unlink(title_file);
	rename(temp_file, title_file);

	/* 对titles文件更新后，还有相应的修改tracks文件 */
	remove_tracks();

	/* 将当前选中的CD清空 */
	current_cd[0] = '\0';
}

/* 删除当前CD唱片中的曲目记录 */
void remove_tracks()
{
	FILE *tracks_fp, *temp_fp;
	char entry[MAX_ENTRY + 1];
	int cat_length;

	if (current_cd[0] == '\0')
	{
		return ;
	}

	cat_length = strlen(current_cat);
	tracks_fp = fopen(tracks_file, "r");
	if (tracks_fp == (FILE *)NULL)
	{
		return ;
	}
	temp_fp = fopen(temp_file, "w");

	while (fgets(entry, MAX_ENTRY, tracks_fp))
	{
		if (strncmp(current_cat, entry, cat_length) != 0)
		{
			fputs(entry, temp_fp);
		}
	}

	fclose(tracks_fp);
	fclose(temp_fp);

	/* 删除tracks文件，之后重命名临时文件 */
	unlink(tracks_file);
	rename(temp_file, tracks_file);
}


/* 对数据库进行扫描并统计出总的唱片数目和曲目数 */
void count_cds()
{
	FILE *titles_fp, *tracks_fp;
	char entry[MAX_ENTRY];
	int titles = 0;
	int tracks = 0;

	titles_fp = fopen(title_file, "r");
	if (titles_fp)
	{
		while (fgets(entry, MAX_ENTRY, titles_fp))
		{
			titles++;
		}
		fclose(titles_fp);
	}
	tracks_fp = fopen(tracks_file, "r");
	if (tracks_fp)
	{
		while (fgets(entry, MAX_ENTRY, tracks_fp))
		{
			tracks++;
		}
		fclose(tracks_fp);
	}
	mvprintw(ERROR_LINE, 0, "Database contains %d titles, with a total of %d tracks.", titles, tracks);
	get_return();
}


/* 提示用户输入一个字符串，根据其在数据库中搜索，并把找到的CD标题放入全局变量current_cd中 */
void find_cd()
{
	char match[MAX_STRING], entry[MAX_ENTRY];
	FILE *titles_fp;
	int count = 0;
	char *found, *title, *catalog;

	mvprintw(Q_LINE, 0, "Enter a string to search for in CD titles: ");
	get_string(match);

	titles_fp = fopen(title_file, "r");
	if (titles_fp)
	{
		while (fgets(entry, MAX_ENTRY, titles_fp))
		{
			catalog = entry;
			if (found = strstr(catalog, ","))
			{
				*found = '\0';
				title = found + 1;
				if (found = strstr(title, ","))
				{
					*found = '\0';
					if (found = strstr(title, match))
					{
						count++;
						strcpy(current_cd, title);
						strcpy(current_cat, catalog);
					}
				}
			}
		}
		fclose(titles_fp);
	}
	if (count != 1)
	{
		if (count == 0)
		{
			mvprintw(ERROR_LINE, 0, "Sorry, no matching CD found.");
		}
		if (count > 1)
		{
			mvprintw(ERROR_LINE, 0, "Sorry, match is ambiguous: %d CDs found. ", count);
		}
		current_cd[0] = '\0';
		get_return();
	}
}


/* 把用户选中的CD唱片中的曲目列在屏幕上 */
void list_tracks()
{
	FILE *tracks_fp;
	char entry[MAX_ENTRY];
	int cat_length;
	int lines_op = 0;
	WINDOW *track_pad_ptr;
	int tracks = 0;
	int key;
	int first_line = 0;

	if (current_cd[0] == '\0')
	{
		mvprintw(ERROR_LINE, 0, "You must select a CD first.");
		get_return();
		return ;
	}

	clear_all_screen();
	cat_length = strlen(current_cat);

	tracks_fp = fopen(tracks_file, "r");
	if (!tracks_fp)
	{
		return;
	}
	while (fgets(entry, MAX_ENTRY, tracks_fp))
	{
		if (strncmp(current_cat, entry, cat_length) == 0)
		{
			tracks++;
		}
	}
	fclose(tracks_fp);

	/* 创建一个新的pad，确保即使只有一首曲目，pad也大到保证稍后的prefresh()操作有效 */
	track_pad_ptr = newpad(tracks+1+BOXED_LINES, BOXED_ROWS+1);
	if (!track_pad_ptr)
	{
		return;
	}
	tracks_fp = fopen(tracks_file, "r");
	if (!tracks_fp)
	{
		return;
	}
	mvprintw(4,0, "CD Track Listing\n");
	 
	while (fgets(entry, MAX_ENTRY, tracks_fp))
	{
		if (strncmp(current_cat, entry, cat_length) == 0)
		{
			mvwprintw(track_pad_ptr, lines_op++, 0, "%s", entry+cat_length+1);
		}
	}
	fclose(tracks_fp);

	if (lines_op > BOXED_LINES)
	{
		mvprintw(MESSAGE_LINE, 0, "Cursor keys to scroll, RETURN or q to exit");
	}
	else
	{
		mvprintw(MESSAGE_LINE, 0, "RETURN or q to quit");
	}
	wrefresh(stdscr);
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	key = 0;
	while (key != 'q' && key != KEY_ENTER && key != '\n')
	{
		if (key == KEY_UP)
		{
			if (first_line > 0)
			{
				first_line--;
			}
		}
		if (key == KEY_DOWN)
		{
			if (first_line + BOXED_LINES +1 < tracks)
			{
				first_line++;
			}
		}

		/* 在屏幕上画出pad区域 */
		prefresh(track_pad_ptr, first_line, 0, BOX_LINE_POS, BOX_ROW_POS, BOX_LINE_POS + BOXED_LINES, BOX_ROW_POS + BOXED_ROWS);
		key = getch();
	}
	
	delwin(track_pad_ptr);
	keypad(stdscr, FALSE);
	nocbreak();
	echo();
}


/* 提示用户按下回车键并读取它 */
void get_return()
{
	int ch;
	mvprintw(23, 0, "%s", "Press return");
	refresh();
	while ((ch=getchar())!='\n' && ch!=EOF);
}
