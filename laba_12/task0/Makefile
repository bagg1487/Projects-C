# Собрать многофайловый проект
# Подсказка: Опция -I в gcc помогает решить проблему с поиском заголовочных файлов. (#include "" No such file or directory)
# Для использования make его необходимо установить: sudo apt install make
CC = gcc
CFLAGS = -Wall -Wextra -I ./background -I ./pipes

SRCS = ./app/main.c ./background/background.c ./pipes/read_pipe.c ./pipes/write_pipe.c
OBJS = ${SRCS:.c=.o}
TARGET = task0_binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: rm -f $(OBJS) $(TARGET)
