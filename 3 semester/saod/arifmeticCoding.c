#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <locale.h>

#define MAX_SYMBOLS 256
#define MAX_BLOCK_SIZE 10000
#define ARITH_PRECISION 53
#define SCALE_FACTOR (1ULL << 48)

typedef struct {
    unsigned char symbol;
    int count;
    double probability;
    double cumulative;
    uint64_t low;
    uint64_t high;
} SymbolStats;

typedef struct {
    uint64_t low;
    uint64_t high;
    uint64_t code;
    int bit_length;
    int block_size;
} ArithResult;

void print_text_preview(const char *filename, int preview_chars) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Не удалось открыть файл %s\n", filename);
        return;
    }
    
    printf("\n=== ПРЕДПРОСМОТР ТЕКСТА (первые %d символов) ===\n", preview_chars);
    
    unsigned char buffer[preview_chars + 1];
    size_t bytes_read = fread(buffer, 1, preview_chars, file);
    
    for (size_t i = 0; i < bytes_read; i++) {
        unsigned char c = buffer[i];
        if (c == '\n') printf("\\n");
        else if (c == '\t') printf("\\t");
        else if (c < 32) printf(".");
        else printf("%c", c);
    }
    printf("\n");
    
    fclose(file);
}

void count_symbols(const char *filename, SymbolStats *symbols, int *unique_count, int *total_count, int block_size) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Ошибка открытия файла %s\n", filename);
        exit(1);
    }
    
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        symbols[i].symbol = i;
        symbols[i].count = 0;
    }
    
    unsigned char buffer[MAX_BLOCK_SIZE];
    size_t bytes_read = fread(buffer, 1, block_size, file);
    *total_count = (bytes_read < block_size) ? bytes_read : block_size;
    
    for (int i = 0; i < *total_count; i++) {
        symbols[buffer[i]].count++;
    }
    
    *unique_count = 0;
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (symbols[i].count > 0) {
            (*unique_count)++;
        }
    }
    
    fclose(file);
}

void calculate_probabilities(SymbolStats *symbols, int total_count) {
    double cumulative = 0.0;
    
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (symbols[i].count > 0) {
            symbols[i].probability = (double)symbols[i].count / total_count;
            symbols[i].cumulative = cumulative;
            cumulative += symbols[i].probability;
            
            symbols[i].low = (uint64_t)(symbols[i].cumulative * SCALE_FACTOR);
            symbols[i].high = (uint64_t)((symbols[i].cumulative + symbols[i].probability) * SCALE_FACTOR);
            
            if (symbols[i].high > SCALE_FACTOR) symbols[i].high = SCALE_FACTOR;
        }
    }
}

void arithmetic_encode(const char *filename, int block_size, SymbolStats *symbols, ArithResult *result, int print_details) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Ошибка открытия файла %s\n", filename);
        exit(1);
    }
    
    unsigned char buffer[MAX_BLOCK_SIZE];
    size_t bytes_read = fread(buffer, 1, block_size, file);
    int actual_size = (bytes_read < block_size) ? bytes_read : block_size;
    
    uint64_t low = 0;
    uint64_t high = SCALE_FACTOR;
    uint64_t range = SCALE_FACTOR;
    
    if (print_details) {
        printf("\n=== ПРОЦЕСС СУЖЕНИЯ ИНТЕРВАЛА ===\n");
        printf("Символ | ASCII | Номер | l_i (норм.) | h_i (норм.) | r_i (норм.)\n");
        printf("----------------------------------------------------------------\n");
    }
    
    for (int i = 0; i < actual_size; i++) {
        unsigned char c = buffer[i];
        
        if (symbols[c].count == 0) {
            printf("Ошибка: символ %d не найден в статистике\n", c);
            continue;
        }
        
        uint64_t symbol_low = symbols[c].low;
        uint64_t symbol_high = symbols[c].high;
        
        uint64_t new_low = low + (range * symbol_low) / SCALE_FACTOR;
        uint64_t new_high = low + (range * symbol_high) / SCALE_FACTOR;
        
        if (print_details && i < 10) {
            double norm_low = (double)new_low / SCALE_FACTOR;
            double norm_high = (double)new_high / SCALE_FACTOR;
            double norm_range = norm_high - norm_low;
            
            printf("%6c | %5d | %5d | %.10f | %.10f | %.10f\n",
                   (c >= 32 && c < 127) ? c : '.',
                   c, i+1, norm_low, norm_high, norm_range);
        }
        
        low = new_low;
        high = new_high;
        range = high - low;
        
        if (range == 0) {
            printf("Внимание: нулевой диапазон на символе %d\n", i+1);
            break;
        }
    }
    
    uint64_t code_value = (low + high) / 2;
    
    result->low = low;
    result->high = high;
    result->code = code_value;
    result->block_size = actual_size;
    
    double range_dbl = (double)range / SCALE_FACTOR;
    int bit_length = 0;
    if (range_dbl > 0) {
        bit_length = (int)ceil(-log2(range_dbl));
    }
    
    if (bit_length < 1) bit_length = 1;
    if (bit_length > 64) bit_length = 64;
    
    result->bit_length = bit_length;
    
    fclose(file);
}

void print_code_in_formats(uint64_t code, int bit_length) {
    printf("Десятичный вид (нормализованный): %.15f\n", (double)code / SCALE_FACTOR);
    
    printf("Двоичный вид: ");
    for (int i = bit_length - 1; i >= 0; i--) {
        printf("%ld", (code >> i) & 1);
    }
    printf("\n");
    
    printf("Шестнадцатеричный вид: 0x%016lX\n", code);
}

int find_max_safe_block(const char *filename, SymbolStats *symbols) {
    printf("\n=== ПОИСК МАКСИМАЛЬНОГО РАЗМЕРА БЛОКА ===\n");
    printf("Размер блока | Состояние | Длина кода | Сжатие\n");
    printf("---------------------------------------------\n");
    
    int max_safe = 0;
    
    for (int block_size = 100; block_size <= 2000; block_size += 100) {
        int unique_count, total_count;
        SymbolStats temp_symbols[MAX_SYMBOLS];
        
        count_symbols(filename, temp_symbols, &unique_count, &total_count, block_size);
        
        if (total_count < block_size) {
            printf("%11d | Файл меньше | - | -\n", block_size);
            break;
        }
        
        calculate_probabilities(temp_symbols, total_count);
        
        ArithResult result;
        arithmetic_encode(filename, block_size, temp_symbols, &result, 0);
        
        double compression = ((double)result.bit_length / 8) / total_count * 100;
        
        if (result.high > result.low && result.bit_length > 0) {
            max_safe = block_size;
            printf("%11d | Без потери | %10d | %6.2f%%\n", block_size, result.bit_length, compression);
        } else {
            printf("%11d | Потеря точности | %10d | %6.2f%%\n", block_size, result.bit_length, compression);
            break;
        }
    }
    
    return max_safe;
}

void arithmetic_decode(const char *output_file, SymbolStats *symbols, uint64_t code, int symbol_count) {
    FILE *out = fopen(output_file, "wb");
    if (!out) {
        printf("Ошибка создания файла %s\n", output_file);
        return;
    }
    
    uint64_t low = 0;
    uint64_t high = SCALE_FACTOR;
    uint64_t range = SCALE_FACTOR;
    uint64_t value = code;
    
    unsigned char *decoded = malloc(symbol_count + 1);
    
    for (int i = 0; i < symbol_count; i++) {
        if (range == 0) {
            printf("Ошибка: нулевой диапазон при декодировании\n");
            break;
        }
        
        uint64_t scaled_value = ((value - low) * SCALE_FACTOR) / range;
        
        unsigned char found = 0;
        for (int j = 0; j < MAX_SYMBOLS; j++) {
            if (symbols[j].count > 0) {
                if (scaled_value >= symbols[j].low && scaled_value < symbols[j].high) {
                    decoded[i] = j;
                    found = j;
                    break;
                }
            }
        }
        
        uint64_t new_low = low + (range * symbols[found].low) / SCALE_FACTOR;
        uint64_t new_high = low + (range * symbols[found].high) / SCALE_FACTOR;
        
        low = new_low;
        high = new_high;
        range = high - low;
    }
    
    decoded[symbol_count] = '\0';
    fwrite(decoded, 1, symbol_count, out);
    free(decoded);
    fclose(out);
}

int compare_files(const char *file1, const char *file2, int size) {
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");
    
    if (!f1 || !f2) return -1;
    
    unsigned char b1, b2;
    int errors = 0;
    
    for (int i = 0; i < size; i++) {
        b1 = fgetc(f1);
        b2 = fgetc(f2);
        if (b1 != b2) {
            errors++;
            if (errors <= 5) {
                printf("Различие %d на позиции %d: '%c'(%d) != '%c'(%d)\n", 
                       errors, i, 
                       (b1 >= 32 && b1 < 127) ? b1 : '.', b1,
                       (b2 >= 32 && b2 < 127) ? b2 : '.', b2);
            }
        }
    }
    
    fclose(f1);
    fclose(f2);
    return errors;
}

double calculate_entropy(SymbolStats *symbols, int total_count) {
    double entropy = 0.0;
    
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (symbols[i].count > 0) {
            double p = symbols[i].probability;
            if (p > 0) {
                entropy -= p * log2(p);
            }
        }
    }
    
    return entropy;
}

void analyze_compression_dependency(const char *filename) {
    printf("\n=== ЗАВИСИМОСТЬ КОЭФФИЦИЕНТА СЖАТИЯ ОТ ДЛИНЫ БЛОКА ===\n");
    printf("Размер блока | Длина кода | Сжатие | Теор. предел\n");
    printf("-------------------------------------------------\n");
    
    for (int block_size = 100; block_size <= 1000; block_size += 100) {
        int unique_count, total_count;
        SymbolStats symbols[MAX_SYMBOLS];
        
        count_symbols(filename, symbols, &unique_count, &total_count, block_size);
        
        if (total_count < block_size) break;
        
        calculate_probabilities(symbols, total_count);
        
        double entropy = calculate_entropy(symbols, total_count);
        ArithResult result;
        arithmetic_encode(filename, block_size, symbols, &result, 0);
        
        double compression = ((double)result.bit_length / 8) / total_count * 100;
        double theory_limit = entropy * 100 / 8;
        
        printf("%11d | %10d | %6.2f%% | %6.2f%%\n", block_size, result.bit_length, compression, theory_limit);
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    const char *input_file = "russian_text.txt";
    const char *output_file = "decoded.txt";
    
    printf("Лабораторная работа №11: Арифметическое кодирование\n");
    printf("===================================================\n");
    
    print_text_preview(input_file, 200);
    
    int max_safe = find_max_safe_block(input_file, NULL);
    printf("\nМаксимальный безопасный размер блока: %d символов\n", max_safe);
    
    int block_size = 500;
    printf("\n=== КОДИРОВАНИЕ БЛОКА %d СИМВОЛОВ ===\n", block_size);
    
    int unique_count, total_count;
    SymbolStats symbols[MAX_SYMBOLS];
    
    count_symbols(input_file, symbols, &unique_count, &total_count, block_size);
    printf("Прочитано символов: %d\n", total_count);
    printf("Уникальных символов: %d\n", unique_count);
    
    calculate_probabilities(symbols, total_count);
    
    printf("\n=== СТАТИСТИКА СИМВОЛОВ (первые 20) ===\n");
    printf("Символ | ASCII | Частота | Вероятность | low | high\n");
    printf("----------------------------------------------------\n");
    
    int printed = 0;
    for (int i = 0; i < MAX_SYMBOLS && printed < 20; i++) {
        if (symbols[i].count > 0) {
            printf("%6c | %5d | %7d | %11.6f | %lu | %lu\n",
                   (i >= 32 && i < 127) ? i : '.',
                   i,
                   symbols[i].count,
                   symbols[i].probability,
                   symbols[i].low,
                   symbols[i].high);
            printed++;
        }
    }
    
    double entropy = calculate_entropy(symbols, total_count);
    printf("\nЭнтропия источника: %.4f бит/символ\n", entropy);
    printf("Теоретический предел сжатия: %.2f%%\n", entropy * 100 / 8);
    
    ArithResult result;
    arithmetic_encode(input_file, block_size, symbols, &result, 1);
    
    printf("\n=== РЕЗУЛЬТАТЫ КОДИРОВАНИЯ ===\n");
    printf("Исходный размер: %d байт\n", result.block_size);
    printf("Длина кодового слова: %d бит\n", result.bit_length);
    printf("Размер кода: %.2f байт\n", (double)result.bit_length / 8);
    
    printf("\n=== КОДОВОЕ СЛОВО ===\n");
    print_code_in_formats(result.code, result.bit_length);
    
    double compression = ((double)result.bit_length / 8) / result.block_size * 100;
    printf("\nКоэффициент сжатия: %.2f%%\n", compression);
    
    analyze_compression_dependency(input_file);
    
    printf("\n=== ДЕКОДИРОВАНИЕ ===\n");
    arithmetic_decode(output_file, symbols, result.code, result.block_size);
    
    printf("\n=== СРАВНЕНИЕ ФАЙЛОВ ===\n");
    int errors = compare_files(input_file, output_file, result.block_size);
    if (errors == 0) {
        printf("✓ Декодирование успешно! Файлы идентичны.\n");
    } else {
        printf("✗ Найдено %d различий.\n", errors);
    }
    
    printf("\n=== АНАЛИЗ ЭФФЕКТИВНОСТИ ===\n");
    printf("Размер блока: %d символов\n", result.block_size);
    printf("Энтропия: %.4f бит/символ\n", entropy);
    printf("Теоретический предел: %.2f%%\n", entropy * 100 / 8);
    printf("Достигнутое сжатие: %.2f%%\n", compression);
    printf("Эффективность: %.2f%% от теоретического предела\n", 
           compression / (entropy * 100 / 8) * 100);
    
    return 0;
}
