CC = gcc
CFLAGS = -std=c99 -ggdb3 `pkg-config --cflags --libs gtk+-3.0` 
LDLIBS = `pkg-config --libs gtk+-3.0`
SRCS = main.c briscola.c gtk_engine.c dialogs.c
OBJS= $(SRCS:.c=.o)

briscola: $(OBJS)
	 $(CXX) -o $@ $^ $(LDFLAGS) $(CFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET)
install:
	cp $(TARGET) /usr/local/bin