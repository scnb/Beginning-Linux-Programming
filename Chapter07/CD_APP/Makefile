all:	application

INCLUDE=/usr/include/gdbm
LIBS=-lgdbm_compat -lgdbm

CFLAGS=

app_ui.o: app_ui.c cd_data.h
	gcc $(CFLAGS) -c app_ui.c

cd_access.o: cd_access.c cd_data.h
	gcc $(CFLAGS) -I$(INCLUDE) -c cd_access.c

application:	app_ui.o cd_access.o
	gcc $(CFLAGS) -o application app_ui.o cd_access.o $(LIBS)

clean:
	rm -f application *.o

nodbmfiles:
	rm -f *.dir *.pag
