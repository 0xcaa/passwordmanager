CFILES = $(wildcard *.c)
OBJFILES = $(CFILES:.c=.o)
PREFIX=/usr/local
NAME=passmanager

CC=gcc
LDFLAGS = -lcrypto -lssl
CFLAGS= -c $(LDFLAGS) -g -Wall -pedantic

$(NAME): $(OBJFILES)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY:clean
clean:
	rm -f $(OBJFILES) $(NAME)
install:
	cp $(NAME) $(DESTDIR)$(PREFIX)/bin/$(NAME)
uninstall:
	rm $(NAME) $(DESTDIR)$(PREFIX)/bin/$(NAME) 
