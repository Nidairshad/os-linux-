CC     = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = scheduler
SRCS   = main.c utils.c fcfs.c sjf.c rr.c priority.c compare.c

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)
	@echo "Build successful. Run: ./$(TARGET)"

clean:
	rm -f $(TARGET)
