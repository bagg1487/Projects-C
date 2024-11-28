CC = gcc
CFLAGS = -Wall -Wextra

SRCS = laba_10_main.c laba_10_other.c
OBJS = ${SRCS:.c=.o}
TARGET = prog_laba_10

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: rm -f $(OBJS) $(TARGET)
