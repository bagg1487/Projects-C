#ifndef DATABASE_H
#define DATABASE_H

#include "constants.h"

typedef struct {
    char fio[FIO_UTF8_SIZE];
    char street[STREET_UTF8_SIZE];
    short house;
    short apartment;
    char date[DATE_UTF8_SIZE];
} Record;

int load_database(const char* filename, Record*** records);
void free_database(Record** records, int count);
void print_records_table(Record** records, int start, int end);
int confirm_continue();
void clean_string(char* str);

#endif