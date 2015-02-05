CC = gcc
GTK_CFLAGS = -std=c99 `pkg-config --cflags gtk+-3.0`
GTK_LIBFLAGS = `pkg-config --libs gtk+-3.0`


555timer: main.c
	$(CC) $(GTK_CFLAGS) -c main.c -o 555timer.o
	$(CC) 555timer.o -o 555timer $(GTK_LIBFLAGS)

clean:
	rm *.o *~ 555timer
