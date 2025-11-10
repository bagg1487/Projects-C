#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdint.h>

typedef struct
{
    uint32_t unicode_char;
    long long frequency;
    double probability, cumulative_prob;
    int code_length;
    char binary_code[512];
} Symbol;

typedef struct
{
    Symbol *symbols_array;
    int count, capacity;
} SymbolTable;

static void init_symbol_table(SymbolTable *table)
{
    table->symbols_array = NULL;
    table->count = 0;
    table->capacity = 0;
}

static void add_symbol(SymbolTable *table, Symbol sym)
{
    if (table->count == table->capacity)
    {
        table->capacity = table->capacity ? table->capacity * 2 : 256;
        table->symbols_array = (Symbol *)realloc(table->symbols_array, table->capacity * sizeof(Symbol));
    }
    table->symbols_array[table->count++] = sym;
}

static Symbol *find_symbol(SymbolTable *table, uint32_t unicode_char)
{
    for (int i = 0; i < table->count; i++)
        if (table->symbols_array[i].unicode_char == unicode_char)
            return &table->symbols_array[i];
    return NULL;
}

static inline double calculate_log2(double x) { return log(x) / log(2.0); }

static int compare_frequency_desc(const void *a, const void *b)
{
    const Symbol *x = (const Symbol *)a, *y = (const Symbol *)b;
    if (y->frequency != x->frequency)
        return (y->frequency > x->frequency) ? 1 : -1;
    return (x->unicode_char > y->unicode_char) - (x->unicode_char < y->unicode_char);
}

static int decode_utf8_char(const unsigned char *input, size_t input_len, uint32_t *output_char, size_t *bytes_used)
{
    if (input_len == 0)
        return 0;
    unsigned char first_byte = input[0];
    if (first_byte < 0x80)
    {
        *output_char = first_byte;
        *bytes_used = 1;
        return 1;
    }
    if ((first_byte & 0xE0) == 0xC0)
    {
        if (input_len < 2)
            return 0;
        uint32_t code_point = ((first_byte & 0x1F) << 6) | (input[1] & 0x3F);
        if (code_point < 0x80)
            return 0;
        *output_char = code_point;
        *bytes_used = 2;
        return 1;
    }
    if ((first_byte & 0xF0) == 0xE0)
    {
        if (input_len < 3)
            return 0;
        uint32_t code_point = ((first_byte & 0x0F) << 12) | ((input[1] & 0x3F) << 6) | (input[2] & 0x3F);
        if (code_point < 0x800)
            return 0;
        *output_char = code_point;
        *bytes_used = 3;
        return 1;
    }
    if ((first_byte & 0xF8) == 0xF0)
    {
        if (input_len < 4)
            return 0;
        uint32_t code_point = ((first_byte & 0x07) << 18) | ((input[1] & 0x3F) << 12) | ((input[2] & 0x3F) << 6) | (input[3] & 0x3F);
        if (code_point < 0x10000 || code_point > 0x10FFFF)
            return 0;
        *output_char = code_point;
        *bytes_used = 4;
        return 1;
    }
    return 0;
}

static int encode_utf8_char(uint32_t code_point, char output[5])
{
    if (code_point < 0x80)
    {
        output[0] = (char)code_point;
        output[1] = 0;
        return 1;
    }
    if (code_point < 0x800)
    {
        output[0] = (char)(0xC0 | (code_point >> 6));
        output[1] = (char)(0x80 | (code_point & 0x3F));
        output[2] = 0;
        return 2;
    }
    if (code_point < 0x10000)
    {
        output[0] = (char)(0xE0 | (code_point >> 12));
        output[1] = (char)(0x80 | ((code_point >> 6) & 0x3F));
        output[2] = (char)(0x80 | (code_point & 0x3F));
        output[3] = 0;
        return 3;
    }
    if (code_point <= 0x10FFFF)
    {
        output[0] = (char)(0xF0 | (code_point >> 18));
        output[1] = (char)(0x80 | ((code_point >> 12) & 0x3F));
        output[2] = (char)(0x80 | ((code_point >> 6) & 0x3F));
        output[3] = (char)(0x80 | (code_point & 0x3F));
        output[4] = 0;
        return 4;
    }
    return 0;
}

typedef struct
{
    uint32_t unicode_char;
    int index;
} CharMapping;

static int compare_char_mapping(const void *a, const void *b)
{
    const CharMapping *x = (const CharMapping *)a, *y = (const CharMapping *)b;
    if (x->unicode_char < y->unicode_char)
        return -1;
    if (x->unicode_char > y->unicode_char)
        return 1;
    return 0;
}

static int find_char_index(const CharMapping *mapping, int mapping_size, uint32_t target_char)
{
    int low = 0, high = mapping_size - 1;
    while (low <= high)
    {
        int mid = (low + high) >> 1;
        if (mapping[mid].unicode_char == target_char)
            return mapping[mid].index;
        if (mapping[mid].unicode_char < target_char)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Использование: %s <входной_файл>\n", argv[0]);
        return 1;
    }
    
    FILE *input_file = fopen(argv[1], "rb");
    if (!input_file)
    {
        fprintf(stderr, "Не удалось открыть файл '%s': %s\n", argv[1], strerror(errno));
        return 1;
    }

    SymbolTable symbol_table;
    init_symbol_table(&symbol_table);
    long long total_symbols_count = 0;
    long long total_file_bytes = 0;
    uint32_t first_100_chars[100];
    int first_chars_count = 0;
    long long first_chars_bytes = 0;

    unsigned char file_buffer[1 << 15];
    size_t bytes_read;
    
    while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), input_file)) > 0)
    {
        total_file_bytes += (long long)bytes_read;
        size_t position = 0;
        
        while (position < bytes_read)
        {
            uint32_t current_char;
            size_t char_bytes;
            
            if (!decode_utf8_char(file_buffer + position, bytes_read - position, &current_char, &char_bytes))
            {
                current_char = (uint32_t)file_buffer[position];
                char_bytes = 1;
            }
            
            if (first_chars_count < 100)
            {
                first_100_chars[first_chars_count++] = current_char;
                first_chars_bytes += (long long)char_bytes;
            }
            
            position += char_bytes;
            Symbol *existing_symbol = find_symbol(&symbol_table, current_char);
            
            if (!existing_symbol)
            {
                Symbol new_symbol;
                new_symbol.unicode_char = current_char;
                new_symbol.frequency = 1;
                new_symbol.probability = 0.0;
                new_symbol.cumulative_prob = 0.0;
                new_symbol.code_length = 0;
                new_symbol.binary_code[0] = 0;
                add_symbol(&symbol_table, new_symbol);
            }
            else
            {
                existing_symbol->frequency++;
            }
            
            total_symbols_count++;
        }
    }
    
    fclose(input_file);
    
    if (total_symbols_count == 0)
    {
        fprintf(stderr, "Файл пуст\n");
        free(symbol_table.symbols_array);
        return 1;
    }

    for (int i = 0; i < symbol_table.count; i++)
        symbol_table.symbols_array[i].probability = (double)symbol_table.symbols_array[i].frequency / (double)total_symbols_count;
    
    qsort(symbol_table.symbols_array, symbol_table.count, sizeof(Symbol), compare_frequency_desc);

    double cumulative_sum = 0.0;
    for (int i = 0; i < symbol_table.count; i++)
    {
        symbol_table.symbols_array[i].cumulative_prob = cumulative_sum;
        symbol_table.symbols_array[i].code_length = (int)ceil(-calculate_log2(symbol_table.symbols_array[i].probability));
        cumulative_sum += symbol_table.symbols_array[i].probability;
    }
    
    for (int i = 0; i < symbol_table.count; i++) 
    {
        double current_prob = symbol_table.symbols_array[i].cumulative_prob;
        int code_len = symbol_table.symbols_array[i].code_length;
        
        for (int j = 0; j < code_len; j++)
        {
            current_prob *= 2.0;
            int bit_value = (int)floor(current_prob);
            symbol_table.symbols_array[i].binary_code[j] = bit_value ? '1' : '0';
            if (current_prob > 1.0)
                current_prob -= 1.0;
        }
        symbol_table.symbols_array[i].binary_code[symbol_table.symbols_array[i].code_length] = '\0';
    }

    printf("                        АНАЛИЗ КОДА ШЕННОНА\n");
    printf("Файл: %s\n", argv[1]);
    printf("Всего байт: %lld, Всего символов: %lld, Уникальных символов: %d\n\n", 
           total_file_bytes, total_symbols_count, symbol_table.count);
    
    printf("ТАБЛИЦА СИМВОЛОВ:\n");
    printf("%-12s | %-14s | %-32s | %-5s\n", "Символ", "Вероятность", "Кодовое слово", "Длина");
    
    for (int i = 0; i < symbol_table.count; i++)
    {
        char utf8_char[5] = {0};
        if (!encode_utf8_char(symbol_table.symbols_array[i].unicode_char, utf8_char))
        {
            utf8_char[0] = '?';
            utf8_char[1] = 0;
        }
        int padding = 12 - (int)strlen(utf8_char);
        if (padding < 0) padding = 0;
        
        printf("%s%*s| %-14.8f | %-32s | %-5d\n", 
               utf8_char, padding, "", 
               symbol_table.symbols_array[i].probability, 
               symbol_table.symbols_array[i].binary_code, 
               symbol_table.symbols_array[i].code_length);
    }
    printf("\n");

    double kraft_sum = 0.0, entropy = 0.0, avg_code_length = 0.0;
    for (int i = 0; i < symbol_table.count; i++)
    {
        kraft_sum += pow(2.0, -(double)symbol_table.symbols_array[i].code_length);
        if (symbol_table.symbols_array[i].probability > 0)
        {
            entropy += -symbol_table.symbols_array[i].probability * calculate_log2(symbol_table.symbols_array[i].probability);
            avg_code_length += symbol_table.symbols_array[i].probability * (double)symbol_table.symbols_array[i].code_length;
        }
    }
    
    printf("АНАЛИЗ КОДА:\n");
    printf("%-25s | %-25s | %-25s | %-25s\n", 
           "Сумма Крафта", "Энтропия источника", "Средняя длина кода", "Избыточность кода");
    printf("%-15.10f | %-15.10f | %-15.10f | %-15.10f\n\n", 
           kraft_sum, entropy, avg_code_length, (avg_code_length - entropy));

    CharMapping *char_map = (CharMapping *)malloc(symbol_table.count * sizeof(CharMapping));
    for (int i = 0; i < symbol_table.count; i++)
    {
        char_map[i].unicode_char = symbol_table.symbols_array[i].unicode_char;
        char_map[i].index = i;
    }
    qsort(char_map, symbol_table.count, sizeof(CharMapping), compare_char_mapping);

    size_t output_capacity = 8192;
    size_t output_length = 0;
    char *encoded_bits = (char *)malloc(output_capacity);
    
    for (int i = 0; i < first_chars_count; i++)
    {
        int symbol_index = find_char_index(char_map, symbol_table.count, first_100_chars[i]);
        if (symbol_index < 0)
            continue;
            
        int code_len = symbol_table.symbols_array[symbol_index].code_length;
        if (output_length + (size_t)code_len + 1 > output_capacity)
        {
            while (output_length + (size_t)code_len + 1 > output_capacity)
                output_capacity *= 2;
            encoded_bits = (char *)realloc(encoded_bits, output_capacity);
        }
        memcpy(encoded_bits + output_length, symbol_table.symbols_array[symbol_index].binary_code, (size_t)code_len);
        output_length += (size_t)code_len;
    }
    encoded_bits[output_length] = '\0';

    long long encoded_bit_count = (long long)output_length;
    long long original_bit_count = (long long)first_chars_bytes * 8LL;
    double compression_ratio = encoded_bit_count ? (double)original_bit_count / (double)encoded_bit_count : 0.0;

    printf("РЕЗУЛЬТАТЫ КОДИРОВАНИЯ:\n");
    printf("Закодированная битовая последовательность:\n%s\n\n", encoded_bits);
    printf("Длина закодированной последовательности: %lld бит\n", encoded_bit_count);
    printf("Длина исходной последовательности:       %lld бит\n", original_bit_count);
    printf("Коэффициент сжатия:                      %.6f\n", compression_ratio);

    printf("\nВЫВОДЫ:\n");
    printf("1. Код Шеннона построен для %d уникальных символов\n", symbol_table.count);
    printf("2. Неравенство Крафта выполняется: %s\n", kraft_sum <= 1.0 ? "ДА" : "НЕТ");
    printf("3. Средняя длина кода: %.6f бит (энтропия: %.6f бит)\n", avg_code_length, entropy);
    printf("4. Избыточность кода: %.6f бит/символ\n", avg_code_length - entropy);
    printf("5. Теорема Шеннона выполняется: %s\n", 
           (avg_code_length >= entropy && avg_code_length < entropy + 1) ? "ДА" : "НЕТ");
    printf("6. Коэффициент сжатия: %.6f\n", compression_ratio);

    free(encoded_bits);
    free(char_map);
    free(symbol_table.symbols_array);
    return 0;
}
