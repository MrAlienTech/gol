CC = gcc
SGDIR = sg
CFLAGS = -I$(SGDIR) -std=c18 -O2 -g
WFLAGS = -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings
LDFLAGS = -lX11
objects = main.o gol.o $(SGDIR)/sg.o
executable = gol

all: $(executable)
       
gol: $(objects)
		$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) $(SGDIR)/sg.c gol.c -o gol main.c

clean: 
		$(RM) $(objects) $(executable)		
tar:
		$(MAKE) clean
		tar -zcvf "$(CURDIR).tar.gz" *
