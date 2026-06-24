CC      = gcc

CFLAGS  = -Wall -Wextra -std=c11 \
           $(shell pkg-config --cflags gtk4)

LIBS    = $(shell pkg-config --libs gtk4)

TARGET  = scheduler

SRCS    = main.c utils.c fcfs.c sjf.c rr.c priority.c compare.c

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)
	@echo "Build successful. Run: ./$(TARGET)"

clean:
	rm -f $(TARGET)
