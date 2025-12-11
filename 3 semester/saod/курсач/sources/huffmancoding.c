#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "../headers/constants.h"
#include "../headers/huffman.h"

#define MAX_TREE_NODES 511
#define MAX_CODE_LENGTH 256

typedef struct HuffmanNode {
    unsigned char data;
    unsigned freq;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
} HuffmanNode;

typedef struct {
    unsigned char data;
    char code[MAX_CODE_LENGTH];
    unsigned freq;
} HuffmanCode;

typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    HuffmanNode** array;
} MinHeap;

HuffmanNode* new_node(unsigned char data, unsigned freq) {
    HuffmanNode* temp = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

MinHeap* create_min_heap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffmanNode**)malloc(minHeap->capacity * sizeof(HuffmanNode*));
    return minHeap;
}

void swap_nodes(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* t = *a;
    *a = *b;
    *b = t;
}

void min_heapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < (int)minHeap->size && 
        minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    
    if (right < (int)minHeap->size && 
        minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    
    if (smallest != idx) {
        swap_nodes(&minHeap->array[smallest], &minHeap->array[idx]);
        min_heapify(minHeap, smallest);
    }
}

int is_size_one(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

HuffmanNode* extract_min(MinHeap* minHeap) {
    HuffmanNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    min_heapify(minHeap, 0);
    return temp;
}

void insert_min_heap(MinHeap* minHeap, HuffmanNode* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

void build_min_heap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        min_heapify(minHeap, i);
}

MinHeap* create_and_build_min_heap(unsigned char data[], unsigned freq[], int size) {
    MinHeap* minHeap = create_min_heap(size);
    
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = new_node(data[i], freq[i]);
    
    minHeap->size = size;
    build_min_heap(minHeap);
    
    return minHeap;
}

HuffmanNode* build_huffman_tree(unsigned char data[], unsigned freq[], int size) {
    HuffmanNode *left, *right, *top;
    
    MinHeap* minHeap = create_and_build_min_heap(data, freq, size);
    
    while (!is_size_one(minHeap)) {
        left = extract_min(minHeap);
        right = extract_min(minHeap);
        
        top = new_node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        
        insert_min_heap(minHeap, top);
    }
    
    HuffmanNode* root = extract_min(minHeap);
    free(minHeap->array);
    free(minHeap);
    
    return root;
}

void generate_codes(HuffmanNode* root, char* code, int depth, HuffmanCode* codes, int* index) {
    if (!root) return;
    
    if (!root->left && !root->right) {
        codes[*index].data = root->data;
        strcpy(codes[*index].code, code);
        (*index)++;
        return;
    }
    
    if (root->left) {
        code[depth] = '0';
        code[depth + 1] = '\0';
        generate_codes(root->left, code, depth + 1, codes, index);
    }
    
    if (root->right) {
        code[depth] = '1';
        code[depth + 1] = '\0';
        generate_codes(root->right, code, depth + 1, codes, index);
    }
}

char* cp866_to_symbol(unsigned char c) {
    static char buffer[10];
    
    if (c == '\n') return "\\n";
    if (c == '\t') return "\\t";
    if (c == '\r') return "\\r";
    if (c >= 32 && c <= 126) {
        buffer[0] = '\'';
        buffer[1] = c;
        buffer[2] = '\'';
        buffer[3] = '\0';
        return buffer;
    }
    
    if (c >= 0x80 && c <= 0xAF) {
        const char* cp866_chars[] = {
            [0x80]="А", [0x81]="Б", [0x82]="В", [0x83]="Г", [0x84]="Д",
            [0x85]="Е", [0x86]="Ж", [0x87]="З", [0x88]="И", [0x89]="Й",
            [0x8A]="К", [0x8B]="Л", [0x8C]="М", [0x8D]="Н", [0x8E]="О",
            [0x8F]="П", [0x90]="Р", [0x91]="С", [0x92]="Т", [0x93]="У",
            [0x94]="Ф", [0x95]="Х", [0x96]="Ц", [0x97]="Ч", [0x98]="Ш",
            [0x99]="Щ", [0x9A]="Ъ", [0x9B]="Ы", [0x9C]="Ь", [0x9D]="Э",
            [0x9E]="Ю", [0x9F]="Я", [0xA0]="а", [0xA1]="б", [0xA2]="в",
            [0xA3]="г", [0xA4]="д", [0xA5]="е", [0xA6]="ж", [0xA7]="з",
            [0xA8]="и", [0xA9]="й", [0xAA]="к", [0xAB]="л", [0xAC]="м",
            [0xAD]="н", [0xAE]="о", [0xAF]="п"
        };
        if (c >= 0x80 && c <= 0xAF && cp866_chars[c]) {
            snprintf(buffer, sizeof(buffer), "%s", cp866_chars[c]);
            return buffer;
        }
    }
    
    if (c >= 0xE0 && c <= 0xEF) {
        const char* cp866_chars[] = {
            [0xE0]="р", [0xE1]="с", [0xE2]="т", [0xE3]="у", [0xE4]="ф",
            [0xE5]="х", [0xE6]="ц", [0xE7]="ч", [0xE8]="ш", [0xE9]="щ",
            [0xEA]="ъ", [0xEB]="ы", [0xEC]="ь", [0xED]="э", [0xEE]="ю",
            [0xEF]="я"
        };
        if (cp866_chars[c]) {
            snprintf(buffer, sizeof(buffer), "%s", cp866_chars[c]);
            return buffer;
        }
    }
    
    snprintf(buffer, sizeof(buffer), "0x%02X", c);
    return buffer;
}

void huffman_encode_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Ошибка открытия файла %s\n", filename);
        return;
    }
    
    unsigned freq[256] = {0};
    unsigned char buffer;
    long total_bytes = 0;
    
    fseek(file, 0, SEEK_END);
    total_bytes = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    printf("=== ПОЛНОЕ КОДИРОВАНИЕ ХАФФМАНА ===\n");
    printf("Размер файла: %ld байт\n", total_bytes);
    
    for (long i = 0; i < total_bytes; i++) {
        fread(&buffer, 1, 1, file);
        freq[(unsigned char)buffer]++;
    }
    fseek(file, 0, SEEK_SET);
    
    int unique_count = 0;
    unsigned char symbols[256];
    unsigned frequencies[256];
    
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            symbols[unique_count] = (unsigned char)i;
            frequencies[unique_count] = freq[i];
            unique_count++;
        }
    }
    
    printf("Уникальных символов: %d\n\n", unique_count);
    
    HuffmanNode* root = build_huffman_tree(symbols, frequencies, unique_count);
    
    char temp_code[MAX_CODE_LENGTH];
    HuffmanCode codes[256];
    int code_index = 0;
    
    generate_codes(root, temp_code, 0, codes, &code_index);
    
    for (int i = 0; i < code_index - 1; i++) {
        for (int j = i + 1; j < code_index; j++) {
            if (freq[(unsigned char)codes[j].data] > freq[(unsigned char)codes[i].data]) {
                HuffmanCode temp = codes[i];
                codes[i] = codes[j];
                codes[j] = temp;
            }
        }
    }
    
    long total_original_bits = total_bytes * 8;
    long total_encoded_bits = 0;
    
    printf("ТАБЛИЦА КОДИРОВАНИЯ:\n");
    printf("┌─────────┬────────┬────────────┬─────────────────────────────────────────────┐\n");
    printf("│ Символ  │ Частота│ Длина кода │ Код Хаффмана                                │\n");
    printf("├─────────┼────────┼────────────┼─────────────────────────────────────────────┤\n");
    
    for (int i = 0; i < code_index; i++) {
        unsigned char symbol = codes[i].data;
        char* code = codes[i].code;
        int code_len = strlen(code);
        
        char* symbol_display = cp866_to_symbol(symbol);
        printf("│ %-7s │ %-6d │ %-10d │ %-43s │\n", 
               symbol_display, freq[symbol], code_len, code);
        
        total_encoded_bits += freq[symbol] * code_len;
    }
    
    printf("└─────────┴────────┴────────────┴─────────────────────────────────────────────┘\n\n");
    
    double entropy = 0.0;
    double avg_code_length = 0.0;
    
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            double probability = (double)freq[i] / total_bytes;
            entropy -= probability * log2(probability);
            for (int j = 0; j < code_index; j++) {
                if (codes[j].data == (unsigned char)i) {
                    avg_code_length += probability * strlen(codes[j].code);
                    break;
                }
            }
        }
    }
    
    double compression_ratio = (double)total_original_bits / total_encoded_bits;
    double space_saving = (1.0 - (double)total_encoded_bits / total_original_bits) * 100;
    
    printf("РЕЗУЛЬТАТЫ КОДИРОВАНИЯ:\n");
    printf("├──────────────────────────────────────────────────────┤\n");
    printf("│ Энтропия файла:                 %10.4f бит/символ │\n", entropy);
    printf("│ Средняя длина кода:             %10.4f бит/символ │\n", avg_code_length);
    printf("│ Избыточность кодирования:       %10.4f бит/символ │\n", avg_code_length - entropy);
    printf("│ Исходный размер:                %10ld бит │\n", total_original_bits);
    printf("│ Размер после кодирования:       %10ld бит │\n", total_encoded_bits);
    printf("│ Коэффициент сжатия:             %10.4f : 1 │\n", compression_ratio);
    printf("│ Экономия пространства:          %10.2f%% │\n", space_saving);
    printf("└──────────────────────────────────────────────────────┘\n");
    
    printf("\nПРИМЕР КОДИРОВАНИЯ (первые 100 байт):\n");
    printf("Исходные данные (HEX):\n");
    
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < 100 && i < total_bytes; i++) {
        fread(&buffer, 1, 1, file);
        printf("%02X ", buffer);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    
    printf("\n\nЗакодированные данные (первые 100 байт):\n");
    fseek(file, 0, SEEK_SET);
    
    int encoded_chars = 0;
    for (int i = 0; i < 100 && i < total_bytes; i++) {
        fread(&buffer, 1, 1, file);
        
        for (int j = 0; j < code_index; j++) {
            if (codes[j].data == buffer) {
                printf("%s ", codes[j].code);
                encoded_chars += strlen(codes[j].code);
                if (encoded_chars > 80) {
                    printf("...\n");
                    i = 100;
                    break;
                }
                break;
            }
        }
    }
    
    printf("\n\n");
    
    char encoded_filename[256];
    snprintf(encoded_filename, sizeof(encoded_filename), "%s.huffman", filename);
    
    FILE* encoded_file = fopen(encoded_filename, "wb");
    if (encoded_file) {
        fwrite(&unique_count, sizeof(int), 1, encoded_file);
        
        for (int i = 0; i < code_index; i++) {
            fwrite(&codes[i].data, 1, 1, encoded_file);
            unsigned char code_len = (unsigned char)strlen(codes[i].code);
            fwrite(&code_len, 1, 1, encoded_file);
            fwrite(codes[i].code, 1, code_len, encoded_file);
        }
        
        fseek(file, 0, SEEK_SET);
        unsigned char current_byte = 0;
        int bit_count = 0;
        
        for (long i = 0; i < total_bytes; i++) {
            fread(&buffer, 1, 1, file);
            
            char* code = NULL;
            for (int j = 0; j < code_index; j++) {
                if (codes[j].data == buffer) {
                    code = codes[j].code;
                    break;
                }
            }
            
            if (code) {
                for (int j = 0; code[j]; j++) {
                    current_byte = (current_byte << 1) | (code[j] - '0');
                    bit_count++;
                    
                    if (bit_count == 8) {
                        fwrite(&current_byte, 1, 1, encoded_file);
                        current_byte = 0;
                        bit_count = 0;
                    }
                }
            }
        }
        
        if (bit_count > 0) {
            current_byte <<= (8 - bit_count);
            fwrite(&current_byte, 1, 1, encoded_file);
            fwrite(&bit_count, sizeof(int), 1, encoded_file);
        } else {
            int zero_bits = 0;
            fwrite(&zero_bits, sizeof(int), 1, encoded_file);
        }
        
        fclose(encoded_file);
        printf("Закодированный файл сохранен как: %s\n", encoded_filename);
    }
    
    fclose(file);
    
    void free_tree(HuffmanNode* node) {
        if (!node) return;
        free_tree(node->left);
        free_tree(node->right);
        free(node);
    }
    free_tree(root);
}