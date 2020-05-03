CC = gcc
CFLAGS = -std=c99 -W -Wextra -Werror -Wall -ggdb3 -pedantic `pkg-config --cflags --libs gtk+-3.0` -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion
CFLAGS+= -D__EXTENSIONS__
LDLIBS = `pkg-config --libs gtk+-3.0`
SRCS = main.c deck.c gui.c briscola.c resources.c
OBJS = $(SRCS:.c=.o)

briscola: $(OBJS)
	 $(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)
clean:
	rm -f $(OBJFILES)
install:
	cp $(TARGET) /usr/local/bin
	