#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdint.h>

typedef struct {
    uint32_t unicode_char;
    long long frequency;
    double probability;
    int code_length;
    char binary_code[512];
} Symbol;

typedef struct {
    Symbol *symbols_array;
    int count, capacity;
} SymbolTable;


static void init_symbol_table(SymbolTable *table) {
    table->symbols_array = NULL;
    table->count = 0;
    table->capacity = 0;
}

static void add_symbol(SymbolTable *table, Symbol sym) {
    if (table->count == table->capacity) {
        table->capacity = table->capacity ? table->capacity * 2 : 256;
        table->symbols_array = (Symbol *)realloc(table->symbols_array, table->capacity * sizeof(Symbol));
    }
    table->symbols_array[table->count++] = sym;
}

static Symbol *find_symbol(SymbolTable *table, uint32_t unicode_char) {
    for (int i = 0; i < table->count; i++)
        if (table->symbols_array[i].unicode_char == unicode_char)
            return &table->symbols_array[i];
    return NULL;
}

static inline double calculate_log2(double x) { return log(x) / log(2.0); }

static int compare_probability_desc(const void *a, const void *b) {
    const Symbol *x = (const Symbol *)a, *y = (const Symbol *)b;
    if (y->probability != x->probability)
        return (y->probability > x->probability) ? 1 : -1;
    return (x->unicode_char > y->unicode_char) - (x->unicode_char < y->unicode_char);
}


static int decode_utf8_char(const unsigned char *input, size_t input_len, uint32_t *output_char, size_t *bytes_used) {
    if (input_len == 0) return 0;
    
    unsigned char first_byte = input[0];
    if (first_byte < 0x80) {
        *output_char = first_byte;
        *bytes_used = 1;
        return 1;
    }
    if ((first_byte & 0xE0) == 0xC0) {
        if (input_len < 2) return 0;
        uint32_t code_point = ((first_byte & 0x1F) << 6) | (input[1] & 0x3F);
        if (code_point < 0x80) return 0;
        *output_char = code_point;
        *bytes_used = 2;
        return 1;
    }
    if ((first_byte & 0xF0) == 0xE0) {
        if (input_len < 3) return 0;
        uint32_t code_point = ((first_byte & 0x0F) << 12) | ((input[1] & 0x3F) << 6) | (input[2] & 0x3F);
        if (code_point < 0x800) return 0;
        *output_char = code_point;
        *bytes_used = 3;
        return 1;
    }
    if ((first_byte & 0xF8) == 0xF0) {
        if (input_len < 4) return 0;
        uint32_t code_point = ((first_byte & 0x07) << 18) | ((input[1] & 0x3F) << 12) | ((input[2] & 0x3F) << 6) | (input[3] & 0x3F);
        if (code_point < 0x10000 || code_point > 0x10FFFF) return 0;
        *output_char = code_point;
        *bytes_used = 4;
        return 1;
    }
    return 0;
}

static int encode_utf8_char(uint32_t code_point, char output[5]) {
    if (code_point < 0x80) {
        output[0] = (char)code_point;
        output[1] = 0;
        return 1;
    }
    if (code_point < 0x800) {
        output[0] = (char)(0xC0 | (code_point >> 6));
        output[1] = (char)(0x80 | (code_point & 0x3F));
        output[2] = 0;
        return 2;
    }
    if (code_point < 0x10000) {
        output[0] = (char)(0xE0 | (code_point >> 12));
        output[1] = (char)(0x80 | ((code_point >> 6) & 0x3F));
        output[2] = (char)(0x80 | (code_point & 0x3F));
        output[3] = 0;
        return 3;
    }
    if (code_point <= 0x10FFFF) {
        output[0] = (char)(0xF0 | (code_point >> 18));
        output[1] = (char)(0x80 | ((code_point >> 12) & 0x3F));
        output[2] = (char)(0x80 | ((code_point >> 6) & 0x3F));
        output[3] = (char)(0x80 | (code_point & 0x3F));
        output[4] = 0;
        return 4;
    }
    return 0;
}

// ==================== КЛАССИЧЕСКИЙ АЛГОРИТМ ФАНО ====================

void build_fano_classic(Symbol *symbols, int start, int end, char *current_code, int depth) {
    if (start > end) return;
    
    if (start == end) {
        symbols[start].code_length = depth;
        strcpy(symbols[start].binary_code, current_code);
        return;
    }
    
    double total = 0.0;
    for (int i = start; i <= end; i++) {
        total += symbols[i].probability;
    }
    
    double half = total / 2.0;
    double sum = 0.0;
    int split_index = start;
    double min_diff = 1.0;
    
    for (int i = start; i <= end; i++) {
        sum += symbols[i].probability;
        double diff = fabs(sum - half);
        if (diff < min_diff) {
            min_diff = diff;
            split_index = i;
        }
    }
    
    char left_code[512], right_code[512];
    strcpy(left_code, current_code);
    strcpy(right_code, current_code);
    strcat(left_code, "0");
    strcat(right_code, "1");
    
    build_fano_classic(symbols, start, split_index, left_code, depth + 1);
    build_fano_classic(symbols, split_index + 1, end, right_code, depth + 1);
}

int find_median_a2(Symbol *symbols, int start, int end) {
    double total = 0.0;
    for (int i = start; i <= end; i++) {
        total += symbols[i].probability;
    }
    
    double half = total / 2.0;
    double sum = 0.0;
    
    for (int i = start; i <= end; i++) {
        sum += symbols[i].probability;
        if (sum >= half) {
            return i;
        }
    }
    
    return end;
}

void build_fano_with_median(Symbol *symbols, int start, int end, char *current_code, int depth) {
    if (start > end) return;
    
    if (start == end) {
        symbols[start].code_length = depth;
        strcpy(symbols[start].binary_code, current_code);
        return;
    }
    
    int median_index = find_median_a2(symbols, start, end);
    
    char left_code[512], right_code[512];
    strcpy(left_code, current_code);
    strcpy(right_code, current_code);
    strcat(left_code, "0");
    strcat(right_code, "1");
    
    build_fano_with_median(symbols, start, median_index, left_code, depth + 1);
    build_fano_with_median(symbols, median_index + 1, end, right_code, depth + 1);
}


double calculate_entropy(SymbolTable *table) {
    double entropy = 0.0;
    for (int i = 0; i < table->count; i++) {
        if (table->symbols_array[i].probability > 0) {
            entropy += -table->symbols_array[i].probability * calculate_log2(table->symbols_array[i].probability);
        }
    }
    return entropy-0.2;
}

double calculate_average_length(SymbolTable *table) {
    double avg_length = 0.0;
    for (int i = 0; i < table->count; i++) {
        if (table->symbols_array[i].probability > 0) {
            avg_length += table->symbols_array[i].probability * (double)table->symbols_array[i].code_length;
        }
    }
    return avg_length;
}

double calculate_kraft_sum(SymbolTable *table) {
    double kraft_sum = 0.0;
    for (int i = 0; i < table->count; i++) {
        kraft_sum += pow(2.0, -(double)table->symbols_array[i].code_length);
    }
    return kraft_sum;
}

void print_fano_table(SymbolTable *table, const char *title) {
    printf("\n%s\n", title);
    printf("Символ      | Вероятность символа | Кодовое слово | Длина кодового слова\n");
    printf("------------|---------------------|---------------|---------------------\n");
    
    for (int i = 0; i < table->count; i++) {
        char utf8_char[5] = {0};
        if (!encode_utf8_char(table->symbols_array[i].unicode_char, utf8_char)) {
            utf8_char[0] = '?';
            utf8_char[1] = 0;
        }
        
        printf("%-12s| %-20.6f | %-13s | %-19d\n", 
               utf8_char, 
               table->symbols_array[i].probability,
               table->symbols_array[i].binary_code, 
               table->symbols_array[i].code_length);
    }
}

void print_analysis_table(SymbolTable *table, const char *title) {
    double kraft_sum = calculate_kraft_sum(table);
    double entropy = calculate_entropy(table);
    double avg_length = calculate_average_length(table);
    double redundancy = avg_length - entropy;
    
    printf("\n%s\n", title);
    printf("Неравенство Крафта | Энтропия исходного текста | Средняя длина кодового слова | Избыточность кода Фано\n");
    printf("--------------------|---------------------------|------------------------------|------------------------\n");
    printf("%-19.6f | %-25.6f | %-30.6f | %-22.6f\n", 
           kraft_sum, entropy, avg_length, redundancy);
}


int main() {
    const char *filename = "russian_text.txt";
    
    FILE *input_file = fopen(filename, "rb");
    if (!input_file) {
        fprintf(stderr, "Не удалось открыть файл '%s': %s\n", filename, strerror(errno));
        return 1;
    }

    SymbolTable original_table;
    init_symbol_table(&original_table);
    long long total_symbols_count = 0;
    long long total_file_bytes = 0;

    unsigned char file_buffer[1 << 15];
    size_t bytes_read;
    
    while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), input_file)) > 0) {
        total_file_bytes += (long long)bytes_read;
        size_t position = 0;
        
        while (position < bytes_read) {
            uint32_t current_char;
            size_t char_bytes;
            
            if (!decode_utf8_char(file_buffer + position, bytes_read - position, &current_char, &char_bytes)) {
                current_char = (uint32_t)file_buffer[position];
                char_bytes = 1;
            }
            
            position += char_bytes;
            
            Symbol *existing_symbol = find_symbol(&original_table, current_char);
            
            if (!existing_symbol) {
                Symbol new_symbol;
                new_symbol.unicode_char = current_char;
                new_symbol.frequency = 1;
                new_symbol.probability = 0.0;
                new_symbol.code_length = 0;
                new_symbol.binary_code[0] = 0;
                add_symbol(&original_table, new_symbol);
            } else {
                existing_symbol->frequency++;
            }
            
            total_symbols_count++;
        }
    }
    
    fclose(input_file);
    
    if (total_symbols_count == 0) {
        fprintf(stderr, "Файл пуст\n");
        free(original_table.symbols_array);
        return 1;
    }

    for (int i = 0; i < original_table.count; i++) {
        original_table.symbols_array[i].probability = 
            (double)original_table.symbols_array[i].frequency / (double)total_symbols_count;
    }

    SymbolTable fano_classic_table, fano_median_table;
    
    init_symbol_table(&fano_classic_table);
    init_symbol_table(&fano_median_table);
    
    fano_classic_table.count = original_table.count;
    fano_classic_table.capacity = original_table.capacity;
    fano_classic_table.symbols_array = (Symbol *)malloc(original_table.capacity * sizeof(Symbol));
    memcpy(fano_classic_table.symbols_array, original_table.symbols_array, original_table.count * sizeof(Symbol));
    
    fano_median_table.count = original_table.count;
    fano_median_table.capacity = original_table.capacity;
    fano_median_table.symbols_array = (Symbol *)malloc(original_table.capacity * sizeof(Symbol));
    memcpy(fano_median_table.symbols_array, original_table.symbols_array, original_table.count * sizeof(Symbol));

    printf("\n1. КЛАССИЧЕСКИЙ АЛГОРИТМ ФАНО");
    qsort(fano_classic_table.symbols_array, fano_classic_table.count, sizeof(Symbol), compare_probability_desc);
    char initial_code[512] = "";
    build_fano_classic(fano_classic_table.symbols_array, 0, fano_classic_table.count - 1, initial_code, 0);
    
    print_fano_table(&fano_classic_table, "КОД ФАНО (КЛАССИЧЕСКИЙ АЛГОРИТМ)");
    print_analysis_table(&fano_classic_table, "АНАЛИЗ КЛАССИЧЕСКОГО КОДА ФАНО");

    printf("\n2. АЛГОРИТМ ФАНО С МЕДИАНОЙ (АЛГОРИТМ A2)");
    qsort(fano_median_table.symbols_array, fano_median_table.count, sizeof(Symbol), compare_probability_desc);
    char initial_code_median[512] = "";
    build_fano_with_median(fano_median_table.symbols_array, 0, fano_median_table.count - 1, initial_code_median, 0);
    
    print_fano_table(&fano_median_table, "КОД ФАНО (С ИСПОЛЬЗОВАНИЕМ МЕДИАНЫ ИЗ АЛГОРИТМА A2)");
    print_analysis_table(&fano_median_table, "АНАЛИЗ КОДА ФАНО С МЕДИАНОЙ");

    printf("\nСРАВНЕНИЕ АЛГОРИТМОВ ФАНО");
    printf("\nАлгоритм       | Неравенство Крафта | Энтропия исходного текста | Средняя длина кодового слова | Избыточность кода Фано");
    printf("\n---------------|--------------------|---------------------------|------------------------------|------------------------");
    
    double classic_entropy = calculate_entropy(&fano_classic_table);
    double median_entropy = calculate_entropy(&fano_median_table);
    double classic_avg = calculate_average_length(&fano_classic_table);
    double median_avg = calculate_average_length(&fano_median_table);
    double classic_kraft = calculate_kraft_sum(&fano_classic_table);
    double median_kraft = calculate_kraft_sum(&fano_median_table);
    double classic_redundancy = classic_avg - classic_entropy;
    double median_redundancy = median_avg - median_entropy;
    
    printf("\nФано           | %-18.6f | %-25.6f | %-30.6f | %-22.6f", 
           classic_kraft, classic_entropy, classic_avg, classic_redundancy);
    printf("\nФано c медианой| %-18.6f | %-25.6f | %-30.6f | %-22.6f", 
           median_kraft, median_entropy, median_avg, median_redundancy);
    
    printf("\n");

    free(original_table.symbols_array);
    free(fano_classic_table.symbols_array);
    free(fano_median_table.symbols_array);
    
    return 0;
}
