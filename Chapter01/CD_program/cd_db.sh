#! /bin/sh

# 对程序中出现的几个变量进行说明：
# cdcatnum: 新增/当前CD的目录编号
# cdtitle:  新增/当前CD的标题
# cdtype：  新增/当前CD的类型
# cdac:		新增/当前CD的艺术家
# cdtrack： 新增/当前CD的某个曲目的编号
# cdttitle：新增/当前CD的某个曲目的名字

menu_choice=""
current_cd=""
title_file="title.cdb"
tracks_file="tracks.cdb"
temp_file=/tmp/cdb.$$
trap 'rm -f $temp_file' EXIT

# Functions

get_return()
{
	echo "Press return \c"
	read x
	return 0
}

get_confirm()
{
	echo "Are you sure? \c"
	while true
	do
		read x
		case "$x" in
			y | Yes | Y | yes )
				return 0;;
			n | no | N | NO | No )
				echo 
				echo "Cancelled"
				return 1;;
			* )
				echo "Please enter yes or no";;
		esac
	done
}

# The main menu.
set_menu_choice()
{
	clear
	echo "Options :-"
	echo 
	echo "	a) Add new CD"
	echo "	f) Find CD"
	echo "	c) Count the CDs and tracks in the catalog"
	if [ "$cdcatnum" != "" ]	# 当前选中某个CD时，以下几个选项才会出现
	then
		echo "	l) List tracks on $cdtitle"
		echo "	r) Remove $cdtitle"
		echo "	u) Update track informatiion for $cdtitle"
	fi

	echo "	q) Quit"
	echo 
	echo "Please enter choice then press return \c"
	read menu_choice
	return
}

# insert new CD title to title_file
insert_title()
{
	echo $* >> $title_file
	return
}

# insert new CD track to track_file
insert_track()
{
	echo $* >> $tracks_file
}

add_record_tracks()
{
	echo "Enter track information for this CD"
	echo "When no more tracks enter q"
	cdtrack=1		# 新增CD中的当前音轨/曲目(一个CD上有很多音轨)
	cdttitle=""
	while [ "$cdttitle" != "q" ]
	do
		echo "Track $cdtrack, track file? \c"
		read tmp
		cdttitle=${tmp%%,*}
		if [ "$tmp" != "$cdttitle" ]
		then
			echo "Sorry, no commas allowed"
			continue
		fi
		if [ -n "$cdttitle" ]
		then
			if [ "$cdttitle" != "q" ]
			then
				insert_track $cdcatnum, $cdtrack, $cdttitle
			fi
		else
			cdtrack=$((cdtrack-1))
		fi
		cdtrack=$((cdtrack+1))
	done
}


# input new CD's information

add_records()
{
	echo "Enter catalog name \c"
	read tmp
	cdcatnum=${tmp%%,*}		# 输入新增CD的目录编号
	echo "Enter title \c"
	read tmp
	cdtitle=${tmp%%,*}		# 输入新增CD的标题
	echo "Enter type \c"
	read tmp
	cdtype=${tmp%%,*}		# 输入新增CD的类型
	echo "Enter artist/composer \c"
	read tmp
	cdac=${tmp%%,*}			# 输入新增CD的艺术家

	echo "About to add new entry"
	echo "$cdcatnum $cdtitle $cdtype $cdac"

	# If user confirmed, then applied it to the titles file
	if get_confirm
	then
		insert_title $cdcatnum, $cdtitle, $cdtype, $cdac
		add_record_tracks # 上面对title_file操作完毕后，再添加CD中的曲目
	else
		remove_records
	fi
	return
}

# use command `grep` to find CDs' information 

find_cd()
{
	if [ "$1" = "n" ] # $1是find_cd()的第一个参数，在调用该函数时默认是y
	then
		asklist=n
	else
		asklist=y
	fi
	cdcatnum=""
	# 搜索的是title.cdb文件中的任意一个字符串/字段
	echo "Enter a string to search for in the CD catalog num \c"
	read searchstr
	if [ "$searchstr" = "" ]
	then
		return 0
	fi

	grep "$searchstr" $title_file > $temp_file

	set $(wc -l $temp_file)

	linesfound=$1		#取得上面搜索结果的第一个参数，即匹配的个数
	
	case "$linesfound" in
		0)	echo "Sorry, nothing found"
			get_return
			return 0
			;;
		1)	;;
		2)	echo "Sorry, not unique."
			echo "Found the following"
			cat $temp_file
			get_return
			return 0
	esac

	IFS=","
	# 以逗号为分隔符，从temp_file文件中提取CD的信息，分别是：
	# 目录编号、标题、类型、艺术家
	read cdcatnum cdtitle cdtype cdac < $temp_file
	IFS=" "
	
	if [ -z "$cdcatnum" ]		# 判断该CD是否存在
	then
		echo "Sorry, could not extract catalog field from $temp_file"
		get_return
		return 0
	fi

	echo 
	echo "Catalog number:  $cdcatnum"
	echo "Title: 		   $cdtitle"
	echo "Type: 		   $cdtype"
	echo "Artist/Composer: $cdac"
	echo
	get_return

	if [ "$asklist" = "y" ]
	then
		echo "View tracks for this CD? \c"
		read x
		if [ "$x" = "y" ]
		then
			echo 
			list_tracks
			echo 
		fi
	fi
	return 1
}


# update_cd函数用于重新输入CD唱片的所有曲目资料

update_cd()
{
	if [ -z "$cdcatnum" ]	# 在这里cdcatnum是由find_cd选中的或由Add new CD 添加的cd
	then
		echo "You must select a CD first"
		find_cd n
	fi

	if [ -n "$cdcatnum" ]	# 若当前选中的CD存在
	then
		echo "Current tracks are :-"
		list_tracks	# 调用list_tracks函数，显示当前CD中的所有曲目
		echo 
		echo "This will re-enter the tracks for $cdtitle"
		get_confirm &&
			{
				#下面的语句搜寻以$cdcatnum的内容开头，且紧跟着一个逗号的行，将其内容重定向到temp_file中
				# 并且必须使用一个临时文件来完成此项工作，否则导致grep命令执行之前，tracks_file文件被重定向操作符设置为空文件
				# -v选项：骚操作，对匹配模式取反，只匹配不符合匹配模式的
				grep -v "^${cdcatnum}," $tracks_file> $temp_file
				mv $temp_file $tracks_file #将文件中原来的内容写回原文件
				echo 
				add_record_tracks
			}
	fi
	return
}


# count_cds fastly count the number of CDs in the database and sum of tracks

count_cds()
{
	set $(wc -l $title_file)
	num_titles=$1	#比较好理解，使用wc命令统计title_file中有几行
	set $(wc -l $tracks_file)
	num_tracks=$1	#原理同上
	echo "found $num_titles CDs, with a total of $num_tracks tracks"
	get_return 
	return
}

# remove_records delete entry from database

remove_records()
{
	if [ -z "$cdcatnum" ]	#先检测是否已经选择了一个CD
	then
		echo "You must select a CD first"
		find_cd n			#带有参数n，表示只搜索(选中)，不显示曲目
	fi

	if [ -n "$cdcatnum" ]
	then
		echo "You are about to delete $cdtitle"
		get_confirm && 
		{
			# -v表示对匹配模式取反，即只选中不符合匹配模式的行，相当于把符合匹配的行删除了，真是骚操作
			grep -v "^${cdcatnum}," $title_file > $temp_file
			mv $temp_file $title_file
			grep -v "^${cdcatnum}," $tracks_file > $temp_file
			mv $temp_file $title_file
			cdcatnum=""
			echo "Entry removed"
		}
		get_return
	fi
	return
}


list_tracks()
{
	if [ "$cdcatnum" = "" ]
	then
		echo "no CD selected yet"
		return
	else
		grep "^${cdcatnum}," $tracks_file > $temp_file
		num_tracks=$(wc -l $temp_file)
		if [ "$num_tracks" = "0" ]
		then
			echo "no tracks found for $cdtitle"
		else
			{
				echo 
				echo "$cdtitle :-"
				echo
				cut -f 2- -d , $temp_file
				echo
			} | ${PAGER:-more}
	 	 fi
	 fi
	 get_return
	 return
}


rm -f $temp_file
if [ ! -f $title_file ]
then
	touch $title_file
fi

if [ ! -f $tracks_file ]
then
	touch $tracks_file
fi

# Now is the main program

clear
echo
echo
echo "				Mini CD manager"
sleep 1

quit=n

while [ "$quit" != "y" ];
do
    set_menu_choice
	case "$menu_choice" in
		a)	add_records;;
		r)	remove_records;;
		f)	find_cd y;;	# 默认带参数y，表示要显示曲目列表
		u) 	update_cd;;
		c)	count_cds;;
		l)	list_tracks;;
		b)	
			echo
			more $title_file
			echo
			get_return;;
		 q | Q) quit=y;;
		 *)	echo "Sorry, choice not recognized";;
	 esac
 done

 # when user choose to quit, tidy up and leave

 rm -f $tmep_file
 echo "Finished"
 exit 0
