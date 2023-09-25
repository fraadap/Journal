CC = gcc
CFLAGS = -W

TARGET = journal

SRCS = journal.c struct.c manage_text.c
HDRS = manage_text.h struct.h

$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

.PHONY: clean

clean:
	rm -f $(TARGET)
