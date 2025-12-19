#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

typedef uint32_t code_value;
typedef uint64_t wide_value;

#define CODE_VALUE_BITS 32
#define TOP_VALUE  0xFFFFFFFFu
#define FIRST_QTR (TOP_VALUE / 4 + 1)
#define HALF      (2 * FIRST_QTR)
#define THIRD_QTR (3 * FIRST_QTR)

typedef struct {
    wchar_t ch;
    wide_value freq;
} Symbol;

typedef struct {
    char *data;
    size_t len;
    size_t cap;
    size_t bit_len;  // Фактическая длина в битах
} BitString;

typedef struct {
    const char *bits;
    size_t len;
    size_t pos;
    int bit_pos;  // Текущая позиция в битах
} BitReader;

static void bitstring_init(BitString *bs) {
    bs->data = NULL;
    bs->len = 0;
    bs->cap = 0;
    bs->bit_len = 0;
}

static void bitstring_free(BitString *bs) {
    free(bs->data);
    bs->data = NULL;
    bs->len = 0;
    bs->cap = 0;
    bs->bit_len = 0;
}

static void bitstring_push(BitString *bs, int bit) {
    if (bs->len + 1 >= bs->cap) {
        size_t new_cap = bs->cap ? bs->cap * 2 : 128;
        char *tmp = (char *)realloc(bs->data, new_cap);
        if (!tmp) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
        bs->data = tmp;
        bs->cap = new_cap;
    }
    bs->data[bs->len++] = (bit ? '1' : '0');
    bs->bit_len++;
}

static void bitstring_terminate(BitString *bs) {
    // Просто добавляем нулевой терминатор для строки
    if (bs->len + 1 >= bs->cap) {
        size_t new_cap = bs->cap + 1;
        char *tmp = (char *)realloc(bs->data, new_cap);
        if (!tmp) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
        bs->data = tmp;
        bs->cap = new_cap;
    }
    bs->data[bs->len] = '\0';
}

static void bitreader_init(BitReader *br, const char *bits, size_t bit_len) {
    br->bits = bits;
    br->len = strlen(bits);  // Длина в символах
    br->pos = 0;
    br->bit_pos = 0;
}

static int bitreader_get(BitReader *br) {
    if (br->bit_pos >= br->len) {
        return 0;  // Возвращаем 0 после конца потока
    }
    char c = br->bits[br->bit_pos++];
    return (c == '1') ? 1 : 0;
}

static int find_symbol_index(Symbol *alphabet, int alpha_size, wchar_t ch) {
    for (int i = 0; i < alpha_size; ++i) {
        if (alphabet[i].ch == ch) return i;
    }
    return -1;
}

static void print_symbol_for_table(wchar_t ch) {
    if (ch == L' ') {
        wprintf(L"' '    ");
    } else if (ch == L'\n') {
        wprintf(L"\\n     ");
    } else {
        wprintf(L"%lc      ", ch);
    }
}

static void output_bit_plus_follow(int bit, int *bits_to_follow, BitString *bs) {
    bitstring_push(bs, bit);
    while (*bits_to_follow > 0) {
        bitstring_push(bs, 1 - bit);
        (*bits_to_follow)--;
    }
}

static void arithmetic_encode(Symbol *alphabet, int alpha_size,
                              wide_value *cum_freq, wide_value total_freq,
                              const int *symbol_idx, int message_len,
                              BitString *out_bits) {
    (void)alpha_size;

    code_value low = 0;
    code_value high = TOP_VALUE;
    int bits_to_follow = 0;

    double scale = (double)TOP_VALUE + 1.0;

    wprintf(L"\nПроцесс арифметического кодирования (сжатие интервала):\n");
    wprintf(L"i      символ l_i                h_i                r_i               \n");

    for (int i = 0; i < message_len; ++i) {
        int s = symbol_idx[i];
        wide_value range = (wide_value)high - (wide_value)low + 1;

        code_value new_high = (code_value)(
            (wide_value)low + (range * cum_freq[s + 1]) / total_freq - 1
        );
        code_value new_low = (code_value)(
            (wide_value)low + (range * cum_freq[s]) / total_freq
        );

        low = new_low;
        high = new_high;

        if (i < 100) {
            double l = (double)low / scale;
            double h = ((double)high + 1.0) / scale;
            double r = h - l;
            wchar_t ch = alphabet[s].ch;
            wprintf(L"%-6d ", i + 1);
            if (ch == L' ') {
                wprintf(L"' '    ");
            } else if (ch == L'\n') {
                wprintf(L"\\n     ");
            } else {
                wprintf(L"%lc       ", ch);
            }
            wprintf(L"%-20.15f %-20.15f %-20.15f\n", l, h, r);
        }

        for (;;) {
            if (high < HALF) {
                output_bit_plus_follow(0, &bits_to_follow, out_bits);
            } else if (low >= HALF) {
                output_bit_plus_follow(1, &bits_to_follow, out_bits);
                low -= HALF;
                high -= HALF;
            } else if (low >= FIRST_QTR && high < THIRD_QTR) {
                bits_to_follow++;
                low -= FIRST_QTR;
                high -= FIRST_QTR;
            } else {
                break;
            }
            low <<= 1;
            high = (high << 1) | 1;
        }
    }

    // Завершающие биты
    bits_to_follow++;
    if (low < FIRST_QTR) {
        output_bit_plus_follow(0, &bits_to_follow, out_bits);
    } else {
        output_bit_plus_follow(1, &bits_to_follow, out_bits);
    }
}

static void arithmetic_decode(Symbol *alphabet, int alpha_size,
                              wide_value *cum_freq, wide_value total_freq,
                              const BitString *in_bits,
                              int message_len,
                              wchar_t *out_message) {
    BitReader br;
    bitreader_init(&br, in_bits->data, in_bits->bit_len);

    code_value low = 0;
    code_value high = TOP_VALUE;
    code_value value = 0;

    // Читаем первые 32 бита для инициализации value
    for (int i = 0; i < CODE_VALUE_BITS; ++i) {
        value = (value << 1) | bitreader_get(&br);
    }

    double scale = (double)TOP_VALUE + 1.0;

    wprintf(L"\nПроцесс арифметического декодирования:\n");
    wprintf(L"i      символ l_i                h_i                r_i               \n");

    for (int i = 0; i < message_len; ++i) {
        wide_value range = (wide_value)high - (wide_value)low + 1;

        // Вычисляем кумулятивную частоту для текущего значения
        wide_value cum = (((wide_value)(value - low) + 1) * total_freq - 1) / range;

        // Находим символ по кумулятивной частоте
        int s = 0;
        while (s < alpha_size && cum_freq[s + 1] <= cum) {
            s++;
        }

        out_message[i] = alphabet[s].ch;

        // Обновляем интервал
        code_value new_high = (code_value)(
            (wide_value)low + (range * cum_freq[s + 1]) / total_freq - 1
        );
        code_value new_low = (code_value)(
            (wide_value)low + (range * cum_freq[s]) / total_freq
        );

        low = new_low;
        high = new_high;

        if (i < 100) {
            double l = (double)low / scale;
            double h = ((double)high + 1.0) / scale;
            double r = h - l;
            wprintf(L"%-6d ", i + 1);
            if (alphabet[s].ch == L' ') {
                wprintf(L"' '    ");
            } else if (alphabet[s].ch == L'\n') {
                wprintf(L"\\n     ");
            } else {
                wprintf(L"%lc       ", alphabet[s].ch);
            }
            wprintf(L"%-20.15f %-20.15f %-20.15f\n", l, h, r);
        }

        // Масштабирование интервала (аналогично кодированию)
        for (;;) {
            if (high < HALF) {
                // Ничего не делаем
            } else if (low >= HALF) {
                low -= HALF;
                high -= HALF;
                value -= HALF;
            } else if (low >= FIRST_QTR && high < THIRD_QTR) {
                low -= FIRST_QTR;
                high -= FIRST_QTR;
                value -= FIRST_QTR;
            } else {
                break;
            }
            
            low <<= 1;
            high = (high << 1) | 1;
            value = (value << 1) | bitreader_get(&br);
        }
    }

    out_message[message_len] = L'\0';
}

static void print_codeword(const BitString *bs, int message_len) {
    size_t codeword_bits = bs->bit_len;
    int orig_bits = message_len * 8;
    
    // Ограничиваем вывод первых 100 бит
    const int max_display_bits = 100;
    char display_buffer[101] = {0};
    
    size_t display_len = codeword_bits;
    int show_ellipsis = 0;
    
    if (codeword_bits > max_display_bits) {
        display_len = max_display_bits;
        show_ellipsis = 1;
    }
    
    // Копируем первые display_len бит
    for (size_t i = 0; i < display_len && i < bs->len; ++i) {
        display_buffer[i] = bs->data[i];
    }
    display_buffer[display_len] = '\0';
    
    printf("\nКодовое слово (двоичное): %s", display_buffer);
    if (show_ellipsis) {
        printf("... [и ещё %zu бит]", codeword_bits - display_len);
    }
    printf("\n");
    
    if (codeword_bits > 0) {
        double x = 0.0;
        for (ssize_t i = (ssize_t)codeword_bits - 1; i >= 0; --i) {
            if (bs->data[i] == '1') {
                x = (1.0 + x) / 2.0;
            } else {
                x = x / 2.0;
            }
        }
        printf("Кодовое слово (десятичная дробь): %.15f\n", x);
    }
    printf("Длина кодового слова: %zu бит\n", codeword_bits);
    printf("Длина исходной последовательности блока: %d бит\n", orig_bits);
    if (codeword_bits > 0) {
        double ratio = (double)orig_bits / (double)codeword_bits;
        printf("Коэффициент сжатия блока orig/encoded: %.6f\n", ratio);
    }
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");

    if (argc < 2) {
        fprintf(stderr, "Usage: %s input.txt [block_size]\n", argv[0]);
        return 1;
    }

    int block_size = 256;
    if (argc >= 3) {
        int v = atoi(argv[2]);
        if (v > 0) block_size = v;
    }

    const int MAX_TEXT = 200000;
    wchar_t *text = (wchar_t *)malloc(sizeof(wchar_t) * MAX_TEXT);
    if (!text) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        free(text);
        return 1;
    }

    fwide(f, 1);

    int len = 0;
    wint_t wc;
    while ((wc = fgetwc(f)) != WEOF) {
        if (len >= MAX_TEXT) break;
        text[len++] = (wchar_t)wc;
    }
    fclose(f);

    if (len == 0) {
        fprintf(stderr, "Empty file\n");
        free(text);
        return 1;
    }

    printf("Общая длина файла: %d символов\n", len);
    printf("Размер блока для кодирования: %d символов\n", block_size);

    unsigned long long total_orig_bits = 0;
    unsigned long long total_encoded_bits = 0;

    int block_index = 0;
    int pos = 0;

    while (pos < len) {
        int message_len = block_size;
        if (pos + message_len > len) {
            message_len = len - pos;
        }

        printf("\n========================================\n");
        printf("Блок %d: позиции [%d, %d), длина %d символов\n",
               block_index + 1, pos, pos + message_len, message_len);

        Symbol *alphabet = (Symbol *)malloc(sizeof(Symbol) * message_len);
        if (!alphabet) {
            fprintf(stderr, "Out of memory\n");
            free(text);
            return 1;
        }
        int alpha_size = 0;

        for (int i = 0; i < message_len; ++i) {
            wchar_t ch = text[pos + i];
            int idx = find_symbol_index(alphabet, alpha_size, ch);
            if (idx == -1) {
                alphabet[alpha_size].ch = ch;
                alphabet[alpha_size].freq = 1;
                alpha_size++;
            } else {
                alphabet[idx].freq++;
            }
        }

        for (int i = 0; i < alpha_size - 1; ++i) {
            for (int j = i + 1; j < alpha_size; ++j) {
                if (alphabet[i].ch > alphabet[j].ch) {
                    Symbol tmp = alphabet[i];
                    alphabet[i] = alphabet[j];
                    alphabet[j] = tmp;
                }
            }
        }

        wide_value total_freq = 0;
        for (int i = 0; i < alpha_size; ++i) {
            total_freq += alphabet[i].freq;
        }

        wprintf(L"\nАлфавит блока и вероятности:\n");
        wprintf(L"символ | p_i         \n");
        wprintf(L"---------+-------------\n\n");
        for (int i = 0; i < alpha_size; ++i) {
            double p = (double)alphabet[i].freq / (double)total_freq;
            print_symbol_for_table(alphabet[i].ch);
            wprintf(L"| %0.8f  \n", p);
        }

        wide_value *cum_freq = (wide_value *)malloc(sizeof(wide_value) * (alpha_size + 1));
        if (!cum_freq) {
            fprintf(stderr, "Out of memory\n");
            free(alphabet);
            free(text);
            return 1;
        }

        cum_freq[0] = 0;
        for (int i = 0; i < alpha_size; ++i) {
            cum_freq[i + 1] = cum_freq[i] + alphabet[i].freq;
        }

        int *symbol_idx = (int *)malloc(sizeof(int) * message_len);
        if (!symbol_idx) {
            fprintf(stderr, "Out of memory\n");
            free(cum_freq);
            free(alphabet);
            free(text);
            return 1;
        }

        for (int i = 0; i < message_len; ++i) {
            int idx = find_symbol_index(alphabet, alpha_size, text[pos + i]);
            if (idx == -1) {
                fprintf(stderr, "Internal error: symbol not found in alphabet\n");
                free(symbol_idx);
                free(cum_freq);
                free(alphabet);
                free(text);
                return 1;
            }
            symbol_idx[i] = idx;
        }

        BitString bits;
        bitstring_init(&bits);

        arithmetic_encode(alphabet, alpha_size, cum_freq, total_freq,
                          symbol_idx, message_len, &bits);

        bitstring_terminate(&bits);

        print_codeword(&bits, message_len);

        wchar_t *decoded = (wchar_t *)malloc(sizeof(wchar_t) * (message_len + 1));
        if (!decoded) {
            fprintf(stderr, "Out of memory\n");
            bitstring_free(&bits);
            free(symbol_idx);
            free(cum_freq);
            free(alphabet);
            free(text);
            return 1;
        }

        arithmetic_decode(alphabet, alpha_size, cum_freq, total_freq,
                          &bits, message_len, decoded);

        wprintf(L"\nДекодированная последовательность блока:\n");
        
        // Выводим только первые 100 символов
        int display_len = message_len;
        int max_display = 100;
        int show_ellipsis = 0;
        
        if (message_len > max_display) {
            display_len = max_display;
            show_ellipsis = 1;
        }
        
        // Выводим первые display_len символов
        for (int i = 0; i < display_len; ++i) {
            wchar_t ch = decoded[i];
            if (ch == L'\n') {
                wprintf(L"\\n");
            } else {
                wprintf(L"%lc", ch);
            }
        }
        
        // Если символов больше, выводим многоточие
        if (show_ellipsis) {
            wprintf(L"... [и ещё %d символов]", message_len - max_display);
        }
        wprintf(L"\n");

        int ok = 1;
        for (int i = 0; i < message_len; ++i) {
            if (decoded[i] != text[pos + i]) {
                wprintf(L"Ошибка на позиции %d: ожидался символ '", i);
                if (text[pos + i] == L'\n') wprintf(L"\\n");
                else wprintf(L"%lc", text[pos + i]);
                wprintf(L"', получен символ '");
                if (decoded[i] == L'\n') wprintf(L"\\n");
                else wprintf(L"%lc", decoded[i]);
                wprintf(L"'\n");
                ok = 0;
                break;
            }
        }

        if (ok) {
            wprintf(L"\nПроверка блока %d: декодированный текст совпадает с исходным.\n",
                    block_index + 1);
        } else {
            wprintf(L"\nВНИМАНИЕ: блок %d декодирован с ошибкой!\n", block_index + 1);
        }

        size_t codeword_bits = bits.bit_len;
        total_orig_bits += (unsigned long long)message_len * 8ull;
        total_encoded_bits += (unsigned long long)codeword_bits;

        free(decoded);
        bitstring_free(&bits);
        free(symbol_idx);
        free(cum_freq);
        free(alphabet);

        pos += message_len;
        block_index++;
    }

    printf("\n========================================\n");
    printf("Итоги по всему файлу\n");
    printf("Число блоков: %d\n", block_index);
    printf("Общая длина исходных данных: %llu бит\n",
           (unsigned long long)total_orig_bits);
    printf("Общая длина закодированных данных: %llu бит\n",
           (unsigned long long)total_encoded_bits);
    if (total_orig_bits > 0) {
        double perc = 100.0 * (double)total_encoded_bits / (double)total_orig_bits;
        double inv = (double)total_orig_bits / (double)total_encoded_bits;
        printf("Коэффициент сжатия encoded/orig: %.2f%%\n", perc);
        printf("Коэффициент сжатия orig/encoded: %.6f\n", inv);
    }

    free(text);
    return 0;
}
