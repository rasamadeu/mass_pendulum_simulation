DEBUG	= -g -ggdb -Wall
#DEBUG	= -O3

GTK_INC	= `pkg-config --cflags glib-2.0` `pkg-config --cflags gtk+-2.0`

GTK_CMP	= `pkg-config --cflags gtk+-3.0`
GTK_LIB	= `pkg-config --libs gtk+-3.0`

#LIBS	= `pkg-config --libs glib-2.0` `pkg-config --libs gtk+-2.0` -lm
LIBS	=  $(GTK_LIB) -lm


CFLAGS	= $(DEBUG) $(GTK_CMP)
LFLAGS	= $(DEBUG)

CC	= gcc
RM	= rm
SRC	= ./src/
LIB	= ./lib/
OBJS	= window.o variables.o callback.o Gtk3_CssAux.o

all: comp link

comp: $(OBJS)

link: projecto $(OBJS)

projecto: projecto.o $(OBJS)
	$(CC) $(LFLAGS) -o projecto projecto.o $(OBJS) $(LIBS)

projecto.o: $(SRC)projecto.c
	$(CC) $(CFLAGS) -c $(SRC)projecto.c

callback.o: $(LIB)callback.h $(SRC)callback.c
	$(CC) $(CFLAGS) -c $(SRC)callback.c

variables.o: $(LIB)variables.h $(SRC)variables.c
	$(CC) $(CFLAGS) -c $(SRC)variables.c

window.o: $(LIB)window.h $(SRC)window.c
	$(CC) $(CFLAGS) -c $(SRC)window.c

Gtk3_CssAux.o: $(LIB)Gtk3_CssAux.h $(SRC)Gtk3_CssAux.c
	$(CC) $(CFLAGS) -c $(SRC)Gtk3_CssAux.c

clean:
	$(RM) -f *~ *.o projecto
