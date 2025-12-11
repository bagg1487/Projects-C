#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/database.h"

void cp866_to_utf8(const char* cp866_str, char* utf8_str, int max_len) {
    static const unsigned char cp866_to_utf8_table[256][3] = {
        [0x80] = {0xD0, 0x90, 0}, [0x81] = {0xD0, 0x91, 0}, [0x82] = {0xD0, 0x92, 0},
        [0x83] = {0xD0, 0x93, 0}, [0x84] = {0xD0, 0x94, 0}, [0x85] = {0xD0, 0x95, 0},
        [0x86] = {0xD0, 0x96, 0}, [0x87] = {0xD0, 0x97, 0}, [0x88] = {0xD0, 0x98, 0},
        [0x89] = {0xD0, 0x99, 0}, [0x8A] = {0xD0, 0x9A, 0}, [0x8B] = {0xD0, 0x9B, 0},
        [0x8C] = {0xD0, 0x9C, 0}, [0x8D] = {0xD0, 0x9D, 0}, [0x8E] = {0xD0, 0x9E, 0},
        [0x8F] = {0xD0, 0x9F, 0}, [0x90] = {0xD0, 0xA0, 0}, [0x91] = {0xD0, 0xA1, 0},
        [0x92] = {0xD0, 0xA2, 0}, [0x93] = {0xD0, 0xA3, 0}, [0x94] = {0xD0, 0xA4, 0},
        [0x95] = {0xD0, 0xA5, 0}, [0x96] = {0xD0, 0xA6, 0}, [0x97] = {0xD0, 0xA7, 0},
        [0x98] = {0xD0, 0xA8, 0}, [0x99] = {0xD0, 0xA9, 0}, [0x9A] = {0xD0, 0xAA, 0},
        [0x9B] = {0xD0, 0xAB, 0}, [0x9C] = {0xD0, 0xAC, 0}, [0x9D] = {0xD0, 0xAD, 0},
        [0x9E] = {0xD0, 0xAE, 0}, [0x9F] = {0xD0, 0xAF, 0}, [0xA0] = {0xD0, 0xB0, 0},
        [0xA1] = {0xD0, 0xB1, 0}, [0xA2] = {0xD0, 0xB2, 0}, [0xA3] = {0xD0, 0xB3, 0},
        [0xA4] = {0xD0, 0xB4, 0}, [0xA5] = {0xD0, 0xB5, 0}, [0xA6] = {0xD0, 0xB6, 0},
        [0xA7] = {0xD0, 0xB7, 0}, [0xA8] = {0xD0, 0xB8, 0}, [0xA9] = {0xD0, 0xB9, 0},
        [0xAA] = {0xD0, 0xBA, 0}, [0xAB] = {0xD0, 0xBB, 0}, [0xAC] = {0xD0, 0xBC, 0},
        [0xAD] = {0xD0, 0xBD, 0}, [0xAE] = {0xD0, 0xBE, 0}, [0xAF] = {0xD0, 0xBF, 0},
        [0xE0] = {0xD1, 0x80, 0}, [0xE1] = {0xD1, 0x81, 0}, [0xE2] = {0xD1, 0x82, 0},
        [0xE3] = {0xD1, 0x83, 0}, [0xE4] = {0xD1, 0x84, 0}, [0xE5] = {0xD1, 0x85, 0},
        [0xE6] = {0xD1, 0x86, 0}, [0xE7] = {0xD1, 0x87, 0}, [0xE8] = {0xD1, 0x88, 0},
        [0xE9] = {0xD1, 0x89, 0}, [0xEA] = {0xD1, 0x8A, 0}, [0xEB] = {0xD1, 0x8B, 0},
        [0xEC] = {0xD1, 0x8C, 0}, [0xED] = {0xD1, 0x8D, 0}, [0xEE] = {0xD1, 0x8E, 0},
        [0xEF] = {0xD1, 0x8F, 0}
    };
    
    int utf8_index = 0;
    
    for (int i = 0; cp866_str[i] != '\0' && utf8_index < max_len - 3; i++) {
        unsigned char c = (unsigned char)cp866_str[i];
        
        if (cp866_to_utf8_table[c][0] != 0) {
            if (utf8_index < max_len - 2) {
                utf8_str[utf8_index++] = cp866_to_utf8_table[c][0];
                utf8_str[utf8_index++] = cp866_to_utf8_table[c][1];
            }
        } else if (c >= 32 && c <= 126) {
            utf8_str[utf8_index++] = c;
        } else if (c == 0) {
            break;
        } else {
            utf8_str[utf8_index++] = ' ';
        }
    }
    
    utf8_str[utf8_index] = '\0';
}

void clean_string(char* str) {
    if (!str) return;
    
    int len = strlen(str);
    while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\0')) {
        str[len-1] = '\0';
        len--;
    }
}

int load_database(const char* filename, Record*** records) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Ошибка открытия файла");
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    int record_size = FIO_SIZE + STREET_SIZE + sizeof(short) + sizeof(short) + DATE_SIZE;
    int count = (int)(file_size / record_size);
    
    printf("Размер файла: %ld байт\n", file_size);
    printf("Размер записи: %d байт\n", record_size);
    printf("Количество записей: %d\n", count);
    
    *records = (Record**)malloc((size_t)count * sizeof(Record*));
    if (!*records) {
        fclose(file);
        return 0;
    }
    
    unsigned char* buffer = (unsigned char*)malloc((size_t)record_size);
    if (!buffer) {
        free(*records);
        fclose(file);
        return 0;
    }
    
    int success_count = 0;
    for (int i = 0; i < count; i++) {
        size_t bytes_read = fread(buffer, 1, (size_t)record_size, file);
        if (bytes_read != (size_t)record_size) {
            break;
        }
        
        (*records)[i] = (Record*)malloc(sizeof(Record));
        if (!(*records)[i]) {
            break;
        }
        
        memset((*records)[i], 0, sizeof(Record));
        
        char raw_fio[FIO_SIZE + 1] = {0};
        char raw_street[STREET_SIZE + 1] = {0};
        char raw_date[DATE_SIZE + 1] = {0};
        
        memcpy(raw_fio, buffer, FIO_SIZE);
        raw_fio[FIO_SIZE] = '\0';
        
        memcpy(raw_street, buffer + FIO_SIZE, STREET_SIZE);
        raw_street[STREET_SIZE] = '\0';
        
        memcpy(&(*records)[i]->house, buffer + FIO_SIZE + STREET_SIZE, sizeof(short));
        memcpy(&(*records)[i]->apartment, buffer + FIO_SIZE + STREET_SIZE + sizeof(short), sizeof(short));
        
        memcpy(raw_date, buffer + FIO_SIZE + STREET_SIZE + 2 * sizeof(short), DATE_SIZE);
        raw_date[DATE_SIZE] = '\0';
        
        cp866_to_utf8(raw_fio, (*records)[i]->fio, FIO_UTF8_SIZE);
        cp866_to_utf8(raw_street, (*records)[i]->street, STREET_UTF8_SIZE);
        cp866_to_utf8(raw_date, (*records)[i]->date, DATE_UTF8_SIZE);
        
        clean_string((*records)[i]->fio);
        clean_string((*records)[i]->street);
        clean_string((*records)[i]->date);
        
        success_count++;
    }
    
    free(buffer);
    fclose(file);
    
    printf("Успешно загружено %d записей\n", success_count);
    return success_count;
}

void free_database(Record** records, int count) {
    for (int i = 0; i < count; i++) {
        free(records[i]);
    }
    free(records);
}

void print_records_table(Record** records, int start, int end) {
    printf("┌─────┬────────────────────────────────┬──────────────────┬──────┬─────────┬────────────┐\n");
    printf("│ №   │ ФИО                            │ Улица            │ Дом  │Квартира │ Дата       │\n");
    printf("├─────┼────────────────────────────────┼──────────────────┼──────┼─────────┼────────────┤\n");
    
    for (int i = start; i < end && i < start + 20; i++) {
        printf("│ %-3d │ %-30s │ %-16s │ %-4d │ %-7d │ %-10s │\n",
               i + 1, records[i]->fio, records[i]->street,
               records[i]->house, records[i]->apartment, records[i]->date);
    }
    
    printf("└─────┴────────────────────────────────┴──────────────────┴──────┴─────────┴────────────┘\n");
}

int confirm_continue() {
    printf("Продолжить просмотр? (y/n): ");
    char response = getchar();
    getchar();
    return response == 'y' || response == 'Y';
}
