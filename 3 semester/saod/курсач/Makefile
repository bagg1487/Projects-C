CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iheaders
LDFLAGS = -lm

SOURCES_MAIN = sources/main.c sources/database.c sources/sort.c sources/search.c sources/tree.c sources/huffmancoding.c
HEADERS = headers/*.h

TARGET = kursovaya

$(TARGET): $(SOURCES_MAIN) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES_MAIN) $(LDFLAGS)

all: $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean all