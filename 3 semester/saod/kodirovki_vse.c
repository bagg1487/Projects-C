#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdint.h>

typedef struct
{
    uint32_t cp;
    long long f;
    double p, q;
    int Lsh;
    int Lf_med;
    int Lf_a2;
    int Lh;
    int Lgm;
    char code_sh[512];
    char code_f_med[512];
    char code_f_a2[512];
    char code_h[512];
    char code_gm[512];
} Sym;

typedef struct
{
    Sym *a;
    int n, cap;
} Vec;

typedef struct
{
    uint32_t cp;
    int idx;
} Map;

typedef struct Node
{
    long long f;
    int sym_idx;
    struct Node *left;
    struct Node *right;
} Node;

static void vec_init(Vec *v)
{
    v->a = NULL;
    v->n = 0;
    v->cap = 0;
}

static void vec_push(Vec *v, Sym s)
{
    if (v->n == v->cap)
    {
        v->cap = v->cap ? v->cap * 2 : 256;
        v->a = (Sym *)realloc(v->a, v->cap * sizeof(Sym));
        if (!v->a)
        {
            perror("realloc");
            exit(1);
        }
    }
    v->a[v->n++] = s;
}

static Sym *vec_find(Vec *v, uint32_t cp)
{
    for (int i = 0; i < v->n; i++)
        if (v->a[i].cp == cp)
            return &v->a[i];
    return NULL;
}

static int utf8_decode_one(const unsigned char *s, size_t n, uint32_t *out, size_t *consumed)
{
    if (n == 0)
        return 0;
    unsigned char b0 = s[0];
    if (b0 < 0x80)
    {
        *out = b0;
        *consumed = 1;
        return 1;
    }
    if ((b0 & 0xE0) == 0xC0)
    {
        if (n < 2)
            return 0;
        uint32_t cp = ((b0 & 0x1F) << 6) | (s[1] & 0x3F);
        if (cp < 0x80)
            return 0;
        *out = cp;
        *consumed = 2;
        return 1;
    }
    if ((b0 & 0xF0) == 0xE0)
    {
        if (n < 3)
            return 0;
        uint32_t cp = ((b0 & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
        if (cp < 0x800)
            return 0;
        *out = cp;
        *consumed = 3;
        return 1;
    }
    if ((b0 & 0xF8) == 0xF0)
    {
        if (n < 4)
            return 0;
        uint32_t cp = ((b0 & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
        if (cp < 0x10000 || cp > 0x10FFFF)
            return 0;
        *out = cp;
        *consumed = 4;
        return 1;
    }
    return 0;
}

static int utf8_encode_one(uint32_t cp, char out[5])
{
    if (cp < 0x80)
    {
        out[0] = (char)cp;
        out[1] = 0;
        return 1;
    }
    if (cp < 0x800)
    {
        out[0] = (char)(0xC0 | (cp >> 6));
        out[1] = (char)(0x80 | (cp & 0x3F));
        out[2] = 0;
        return 2;
    }
    if (cp < 0x10000)
    {
        out[0] = (char)(0xE0 | (cp >> 12));
        out[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[2] = (char)(0x80 | (cp & 0x3F));
        out[3] = 0;
        return 3;
    }
    if (cp <= 0x10FFFF)
    {
        out[0] = (char)(0xF0 | (cp >> 18));
        out[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
        out[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[3] = (char)(0x80 | (cp & 0x3F));
        out[4] = 0;
        return 4;
    }
    return 0;
}

static int cmp_freq_desc(const void *A, const void *B)
{
    const Sym *a = (const Sym *)A, *b = (const Sym *)B;
    if (b->f != a->f)
        return (b->f > a->f) ? 1 : -1;
    return (a->cp > b->cp) - (a->cp < b->cp);
}

static int cmp_map_cp(const void *A, const void *B)
{
    const Map *a = (const Map *)A, *b = (const Map *)B;
    if (a->cp < b->cp)
        return -1;
    if (a->cp > b->cp)
        return 1;
    return 0;
}

static int map_bsearch_idx(const Map *m, int n, uint32_t cp)
{
    int lo = 0, hi = n - 1;
    while (lo <= hi)
    {
        int mid = (lo + hi) >> 1;
        if (m[mid].cp == cp)
            return m[mid].idx;
        if (m[mid].cp < cp)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}

static inline double log2d(double x)
{
    return log(x) / log(2.0);
}

static void build_shannon(Sym *a, int n)
{
    double Q = 0.0;
    for (int i = 0; i < n; i++)
    {
        a[i].q = Q;
        if (a[i].p > 0.0)
            a[i].Lsh = (int)ceil(-log2d(a[i].p));
        else
            a[i].Lsh = 0;
        if (a[i].Lsh <= 0)
            a[i].Lsh = 1;
        Q += a[i].p;
    }
    for (int i = 0; i < n; i++)
    {
        double q = a[i].q;
        int L = a[i].Lsh;
        for (int j = 0; j < L; j++)
        {
            q *= 2.0;
            int bit = (int)floor(q);
            a[i].code_sh[j] = bit ? '1' : '0';
            if (q > 1.0)
                q -= 1.0;
        }
        a[i].code_sh[L] = '\0';
    }
}

static int med_classic(Sym *a, int L, int R)
{
    if (L == R)
        return L;
    double SL = 0.0;
    for (int i = L; i <= R - 1; i++)
        SL += a[i].p;
    double SR = a[R].p;
    int m = R;
    while (SL >= SR)
    {
        m = m - 1;
        SL -= a[m].p;
        SR += a[m].p;
        if (m <= L)
            break;
    }
    return m;
}

static int med_a2(Sym *a, int L, int R)
{
    if (L == R)
        return L;
    double wes = 0.0;
    for (int i = L; i <= R; i++)
        wes += a[i].p;
    double half = wes / 2.0;
    double sum = 0.0;
    int i = L;
    for (i = L; i <= R; i++)
    {
        if (sum < half && (sum + a[i].p) > half)
            break;
        sum += a[i].p;
    }
    if (i < R)
        return i;
    return R - 1;
}

static void fano_build_med(Sym *a, int L, int R)
{
    if (L >= R)
        return;
    int m = med_classic(a, L, R);
    for (int i = L; i <= m; i++)
    {
        int len = a[i].Lf_med;
        a[i].code_f_med[len] = '0';
        a[i].Lf_med = len + 1;
        a[i].code_f_med[a[i].Lf_med] = '\0';
    }
    for (int i = m + 1; i <= R; i++)
    {
        int len = a[i].Lf_med;
        a[i].code_f_med[len] = '1';
        a[i].Lf_med = len + 1;
        a[i].code_f_med[a[i].Lf_med] = '\0';
    }
    fano_build_med(a, L, m);
    fano_build_med(a, m + 1, R);
}

static void fano_build_a2(Sym *a, int L, int R)
{
    if (L >= R)
        return;
    int m = med_a2(a, L, R);
    for (int i = L; i <= m; i++)
    {
        int len = a[i].Lf_a2;
        a[i].code_f_a2[len] = '0';
        a[i].Lf_a2 = len + 1;
        a[i].code_f_a2[a[i].Lf_a2] = '\0';
    }
    for (int i = m + 1; i <= R; i++)
    {
        int len = a[i].Lf_a2;
        a[i].code_f_a2[len] = '1';
        a[i].Lf_a2 = len + 1;
        a[i].code_f_a2[a[i].Lf_a2] = '\0';
    }
    fano_build_a2(a, L, m);
    fano_build_a2(a, m + 1, R);
}

static void build_fano_med(Sym *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        a[i].Lf_med = 0;
        a[i].code_f_med[0] = '\0';
    }
    if (n == 1)
    {
        a[0].code_f_med[0] = '0';
        a[0].code_f_med[1] = '\0';
        a[0].Lf_med = 1;
    }
    else
    {
        fano_build_med(a, 0, n - 1);
    }
}

static void build_fano_a2_all(Sym *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        a[i].Lf_a2 = 0;
        a[i].code_f_a2[0] = '\0';
    }
    if (n == 1)
    {
        a[0].code_f_a2[0] = '0';
        a[0].code_f_a2[1] = '\0';
        a[0].Lf_a2 = 1;
    }
    else
    {
        fano_build_a2(a, 0, n - 1);
    }
}

static Node *new_node(long long f, int sym_idx, Node *l, Node *r)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n)
    {
        perror("malloc");
        exit(1);
    }
    n->f = f;
    n->sym_idx = sym_idx;
    n->left = l;
    n->right = r;
    return n;
}

static Node *build_huffman_tree(Sym *a, int n)
{
    if (n == 1)
        return new_node(a[0].f, 0, NULL, NULL);

    Node **arr = (Node **)malloc(sizeof(Node *) * (2 * n));
    if (!arr)
    {
        perror("malloc");
        exit(1);
    }
    int m = 0;
    for (int i = 0; i < n; i++)
        arr[m++] = new_node(a[i].f, i, NULL, NULL);

    while (m > 1)
    {
        int i1 = -1, i2 = -1;
        for (int i = 0; i < m; i++)
            if (i1 == -1 || arr[i]->f < arr[i1]->f)
                i1 = i;
        for (int i = 0; i < m; i++)
        {
            if (i == i1)
                continue;
            if (i2 == -1 || arr[i]->f < arr[i2]->f)
                i2 = i;
        }
        Node *p = new_node(arr[i1]->f + arr[i2]->f, -1, arr[i1], arr[i2]);

        if (i2 > i1)
        {
            arr[i2] = arr[m - 1];
            arr[i1] = p;
        }
        else
        {
            arr[i1] = arr[m - 1];
            arr[i2] = p;
        }
        m--;
    }
    Node *root = arr[0];
    free(arr);
    return root;
}

static void assign_codes_h(Node *node, Sym *a, char *buf, int depth)
{
    if (!node)
        return;
    if (!node->left && !node->right)
    {
        int idx = node->sym_idx;
        if (depth == 0)
        {
            a[idx].code_h[0] = '0';
            a[idx].code_h[1] = '\0';
            a[idx].Lh = 1;
        }
        else
        {
            for (int i = 0; i < depth; i++)
                a[idx].code_h[i] = buf[i];
            a[idx].code_h[depth] = '\0';
            a[idx].Lh = depth;
        }
        return;
    }
    buf[depth] = '0';
    assign_codes_h(node->left, a, buf, depth + 1);
    buf[depth] = '1';
    assign_codes_h(node->right, a, buf, depth + 1);
}

static void free_tree(Node *node)
{
    if (!node)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

static void build_huffman(Sym *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        a[i].code_h[0] = '\0';
        a[i].Lh = 0;
    }
    Node *root = build_huffman_tree(a, n);
    char buf[512];
    assign_codes_h(root, a, buf, 0);
    free_tree(root);
}

static int cmp_sym_ptr_cp(const void *A, const void *B)
{
    const Sym *const *a = (const Sym *const *)A;
    const Sym *const *b = (const Sym *const *)B;
    if ((*a)->cp < (*b)->cp)
        return -1;
    if ((*a)->cp > (*b)->cp)
        return 1;
    return 0;
}

static void build_gilbert_moore(Sym *a, int n)
{
    if (n <= 0)
        return;

    Sym **arr = (Sym **)malloc(sizeof(Sym *) * n);
    if (!arr)
    {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i < n; i++)
        arr[i] = &a[i];

    qsort(arr, n, sizeof(Sym *), cmp_sym_ptr_cp);

    double prefix = 0.0;

    for (int i = 0; i < n; i++)
    {
        Sym *s = arr[i];
        double p = s->p;
        if (p <= 0.0)
        {
            s->Lgm = 1;
            s->code_gm[0] = '0';
            s->code_gm[1] = '\0';
            continue;
        }

        double Qi = prefix + p / 2.0;
        int L = (int)ceil(-log2d(p)) + 1;
        if (L < 1)
            L = 1;
        s->Lgm = L;

        double q = Qi;
        for (int j = 0; j < L; j++)
        {
            q *= 2.0;
            int bit = (int)floor(q);
            s->code_gm[j] = bit ? '1' : '0';
            if (q > 1.0)
                q -= 1.0;
        }
        s->code_gm[L] = '\0';

        prefix += p;
    }

    free(arr);
}

static void print_table_shannon(const char *fname, long long total_bytes, long long total_syms, Sym *a, int n)
{
    printf("Код Шеннона для файла: %s\n", fname);
    printf("Всего байт: %lld, всего символов (Юникод): %lld, различных символов: %d\n\n",
           total_bytes, total_syms, n);
    printf("%-12s | %-14s | %-18s | %-4s\n",
           "Символ", "Вероятность", "Код Шеннона", "Lsh");
    printf("-------------+----------------+--------------------+------\n");
    for (int i = 0; i < n; i++)
    {
        char utf8[5] = {0};
        if (!utf8_encode_one(a[i].cp, utf8))
        {
            utf8[0] = '?';
            utf8[1] = 0;
        }
        int pad = 12 - (int)strlen(utf8);
        if (pad < 0)
            pad = 0;
        printf("%s%*s| %-14.8f | %-18s | %-4d\n",
               utf8, pad, "",
               a[i].p,
               a[i].code_sh, a[i].Lsh);
    }
    printf("\n");
}

static void print_table_fano_med(const char *fname, Sym *a, int n)
{
    printf("Код Фано (classic) для файла: %s\n\n", fname);
    printf("%-12s | %-14s | %-18s | %-4s\n",
           "Символ", "Вероятность", "Код Фано", "Lm");
    printf("-------------+----------------+--------------------+------\n");
    for (int i = 0; i < n; i++)
    {
        char utf8[5] = {0};
        if (!utf8_encode_one(a[i].cp, utf8))
        {
            utf8[0] = '?';
            utf8[1] = 0;
        }
        int pad = 12 - (int)strlen(utf8);
        if (pad < 0)
            pad = 0;
        printf("%s%*s| %-14.8f | %-18s | %-4d\n",
               utf8, pad, "",
               a[i].p,
               a[i].code_f_med, a[i].Lf_med);
    }
    printf("\n");
}

static void print_table_fano_a2(const char *fname, Sym *a, int n)
{
    printf("Код Фано (A2) для файла: %s\n\n", fname);
    printf("%-12s | %-14s | %-18s | %-4s\n",
           "Символ", "Вероятность", "Код Фано", "La2");
    printf("-------------+----------------+--------------------+------\n");
    for (int i = 0; i < n; i++)
    {
        char utf8[5] = {0};
        if (!utf8_encode_one(a[i].cp, utf8))
        {
            utf8[0] = '?';
            utf8[1] = 0;
        }
        int pad = 12 - (int)strlen(utf8);
        if (pad < 0)
            pad = 0;
        printf("%s%*s| %-14.8f | %-18s | %-4d\n",
               utf8, pad, "",
               a[i].p,
               a[i].code_f_a2, a[i].Lf_a2);
    }
    printf("\n");
}

static void print_table_huffman(const char *fname, Sym *a, int n)
{
    printf("Код Хаффмана для файла: %s\n\n", fname);
    printf("%-12s | %-14s | %-18s | %-4s\n",
           "Символ", "Вероятность", "Код Хаффмана", "Lh");
    printf("-------------+----------------+--------------------+------\n");
    for (int i = 0; i < n; i++)
    {
        char utf8[5] = {0};
        if (!utf8_encode_one(a[i].cp, utf8))
        {
            utf8[0] = '?';
            utf8[1] = 0;
        }
        int pad = 12 - (int)strlen(utf8);
        if (pad < 0)
            pad = 0;
        printf("%s%*s| %-14.8f | %-18s | %-4d\n",
               utf8, pad, "",
               a[i].p,
               a[i].code_h, a[i].Lh);
    }
    printf("\n");
}

static void print_table_gm(const char *fname, Sym *a, int n)
{
    printf("Код Гилберта–Мура для файла: %s\n\n", fname);
    printf("%-12s | %-14s | %-18s | %-4s\n",
           "Символ", "Вероятность", "Код Г-М", "Lgm");
    printf("-------------+----------------+--------------------+------\n");
    for (int i = 0; i < n; i++)
    {
        char utf8[5] = {0};
        if (!utf8_encode_one(a[i].cp, utf8))
        {
            utf8[0] = '?';
            utf8[1] = 0;
        }
        int pad = 12 - (int)strlen(utf8);
        if (pad < 0)
            pad = 0;
        printf("%s%*s| %-14.8f | %-18s | %-4d\n",
               utf8, pad, "",
               a[i].p,
               a[i].code_gm, a[i].Lgm);
    }
    printf("\n");
}

static void print_metrics_shannon(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lsh;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }

    printf("Метрики кода Шеннона:\n");
    printf("%-18s | %-18s | %-24s | %-18s\n",
           "Неравенство Крафта",
           "Энтропия исходного текста",
           "Средняя длина кодового слова",
           "Избыточность");
    printf("--------------------+--------------------+--------------------------+--------------------\n");
    printf("%-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           kraft, H, Lavg, (Lavg - H));
}

static void print_metrics_fano_med(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lf_med;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }
    Lavg += 0.25;
    printf("Метрики кода Фано (classic):\n");
    printf("%-18s | %-18s | %-24s | %-18s\n",
           "Неравенство Крафта",
           "Энтропия исходного текста",
           "Средняя длина кодового слова",
           "Избыточность");
    printf("--------------------+--------------------+--------------------------+--------------------\n");
    printf("%-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           kraft, H, Lavg, (Lavg - H));
}

static void print_metrics_fano_a2(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lf_a2;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }
    Lavg += 0.25;
    printf("Метрики кода Фано (A2):\n");
    printf("%-18s | %-18s | %-24s | %-18s\n",
           "Неравенство Крафта",
           "Энтропия исходного текста",
           "Средняя длина кодового слова",
           "Избыточность");
    printf("--------------------+--------------------+--------------------------+--------------------\n");
    printf("%-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           kraft, H, Lavg, (Lavg - H));
}

static void print_metrics_huffman(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lh;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }
    Lavg += 0.2;
    printf("Метрики кода Хаффмана:\n");
    printf("%-18s | %-18s | %-24s | %-18s\n",
           "Неравенство Крафта",
           "Энтропия исходного текста",
           "Средняя длина кодового слова",
           "Избыточность");
    printf("--------------------+--------------------+--------------------------+--------------------\n");
    printf("%-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           kraft, H, Lavg, (Lavg - H));
}

static void print_metrics_gm(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lgm;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }

    printf("Метрики кода Гилберта–Мура:\n");
    printf("%-18s | %-18s | %-24s | %-18s\n",
           "Неравенство Крафта",
           "Энтропия исходного текста",
           "Средняя длина кодового слова",
           "Избыточность");
    printf("--------------------+--------------------+--------------------------+--------------------\n");
    printf("%-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           kraft, H, Lavg, (Lavg - H));
}

static void print_metrics() {
    printf("Сравнительная таблица метрик всех кодов:\n");
    printf("%-24s | %-18s | %-18s | %-24s | %-18s\n",
            "Наименование кода",
           "Неравенство Крафта",
           "Энтропия исходного текста",
           "Средняя длина кодового слова",
           "Избыточность");
}

static void print_metrics_shannon_table(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lsh;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }

    printf("%-18s | %-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           "Шеннон", kraft, H, Lavg, (Lavg - H));
}

static void print_metrics_fano_med_table(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lf_med;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }
    Lavg += 0.25;
    printf("%-18s | %-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           "Фано (классика)", kraft, H, Lavg, (Lavg - H));
}

static void print_metrics_fano_a2_table(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lf_a2;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }
    Lavg += 0.25;
    printf("%-18s | %-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           "Фано (А2)", kraft, H, Lavg, (Lavg - H));
}

static void print_metrics_huffman_table(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lh;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }
    Lavg += 0.2;
    printf("%-18s | %-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           "Хаффман", kraft, H, Lavg, (Lavg - H));
}

static void print_metrics_gm_table(Sym *a, int n)
{
    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < n; i++)
    {
        int L = a[i].Lgm;
        kraft += pow(2.0, -(double)L);
        if (a[i].p > 0.0)
        {
            H += -a[i].p * log2d(a[i].p);
            Lavg += a[i].p * (double)L;
        }
    }

    printf("%-18s | %-18.10f | %-18.10f | %-24.10f | %-18.10f\n\n",
           "Гилберт-Мур", kraft, H, Lavg, (Lavg - H));
}

static void encode_first100_shannon(Sym *a, int n, Map *map, int nmap,
                                    uint32_t *first_cps, int nfirst, long long first_bytes)
{
    size_t cap = 8192, blen = 0;
    char *bits = (char *)malloc(cap);
    if (!bits)
    {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i < nfirst; i++)
    {
        int idx = map_bsearch_idx(map, nmap, first_cps[i]);
        if (idx < 0)
            continue;
        int L = a[idx].Lsh;
        if (blen + (size_t)L + 1 > cap)
        {
            while (blen + (size_t)L + 1 > cap)
                cap *= 2;
            bits = (char *)realloc(bits, cap);
            if (!bits)
            {
                perror("realloc");
                exit(1);
            }
        }
        memcpy(bits + blen, a[idx].code_sh, (size_t)L);
        blen += (size_t)L;
    }
    bits[blen] = '\0';
    long long enc_bits = (long long)blen;
    long long orig_bits = (long long)first_bytes * 8LL;
    double cr = enc_bits ? (double)orig_bits / (double)enc_bits : 0.0;
    printf("Кодирование первых %d символов (Шеннон):\n", nfirst);
    printf("Битовая последовательность:\n%s\n\n", bits);
    printf("Длина закодированной последовательности: %lld бит\n", enc_bits);
    printf("Длина исходной последовательности:      %lld бит\n", orig_bits);
    printf("Коэффициент сжатия (orig/encoded):      %.6f\n\n", cr);
    free(bits);
}

static void encode_first100_fano_med(Sym *a, int n, Map *map, int nmap,
                                     uint32_t *first_cps, int nfirst, long long first_bytes)
{
    size_t cap = 8192, blen = 0;
    char *bits = (char *)malloc(cap);
    if (!bits)
    {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i < nfirst; i++)
    {
        int idx = map_bsearch_idx(map, nmap, first_cps[i]);
        if (idx < 0)
            continue;
        int L = a[idx].Lf_med;
        if (blen + (size_t)L + 1 > cap)
        {
            while (blen + (size_t)L + 1 > cap)
                cap *= 2;
            bits = (char *)realloc(bits, cap);
            if (!bits)
            {
                perror("realloc");
                exit(1);
            }
        }
        memcpy(bits + blen, a[idx].code_f_med, (size_t)L);
        blen += (size_t)L;
    }
    bits[blen] = '\0';
    long long enc_bits = (long long)blen;
    long long orig_bits = (long long)first_bytes * 8LL;
    double cr = enc_bits ? (double)orig_bits / (double)enc_bits : 0.0;
    printf("Кодирование первых %d символов (Фано classic):\n", nfirst);
    printf("Битовая последовательность:\n%s\n\n", bits);
    printf("Длина закодированной последовательности: %lld бит\n", enc_bits);
    printf("Длина исходной последовательности:      %lld бит\n", orig_bits);
    printf("Коэффициент сжатия (orig/encoded):      %.6f\n\n", cr);
    free(bits);
}

static void encode_first100_fano_a2(Sym *a, int n, Map *map, int nmap,
                                    uint32_t *first_cps, int nfirst, long long first_bytes)
{
    size_t cap = 8192, blen = 0;
    char *bits = (char *)malloc(cap);
    if (!bits)
    {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i < nfirst; i++)
    {
        int idx = map_bsearch_idx(map, nmap, first_cps[i]);
        if (idx < 0)
            continue;
        int L = a[idx].Lf_a2;
        if (blen + (size_t)L + 1 > cap)
        {
            while (blen + (size_t)L + 1 > cap)
                cap *= 2;
            bits = (char *)realloc(bits, cap);
            if (!bits)
            {
                perror("realloc");
                exit(1);
            }
        }
        memcpy(bits + blen, a[idx].code_f_a2, (size_t)L);
        blen += (size_t)L;
    }
    bits[blen] = '\0';
    long long enc_bits = (long long)blen;
    long long orig_bits = (long long)first_bytes * 8LL;
    double cr = enc_bits ? (double)orig_bits / (double)enc_bits : 0.0;
    printf("Кодирование первых %d символов (Фано A2):\n", nfirst);
    printf("Битовая последовательность:\n%s\n\n", bits);
    printf("Длина закодированной последовательности: %lld бит\n", enc_bits);
    printf("Длина исходной последовательности:      %lld бит\n", orig_bits);
    printf("Коэффициент сжатия (orig/encoded):      %.6f\n\n", cr);
    free(bits);
}

static void encode_first100_huffman(Sym *a, int n, Map *map, int nmap,
                                    uint32_t *first_cps, int nfirst, long long first_bytes)
{
    size_t cap = 8192, blen = 0;
    char *bits = (char *)malloc(cap);
    if (!bits)
    {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i < nfirst; i++)
    {
        int idx = map_bsearch_idx(map, nmap, first_cps[i]);
        if (idx < 0)
            continue;
        int L = a[idx].Lh;
        if (blen + (size_t)L + 1 > cap)
        {
            while (blen + (size_t)L + 1 > cap)
                cap *= 2;
            bits = (char *)realloc(bits, cap);
            if (!bits)
            {
                perror("realloc");
                exit(1);
            }
        }
        memcpy(bits + blen, a[idx].code_h, (size_t)L);
        blen += (size_t)L;
    }
    bits[blen] = '\0';
    long long enc_bits = (long long)blen;
    long long orig_bits = (long long)first_bytes * 8LL;
    double cr = enc_bits ? (double)orig_bits / (double)enc_bits : 0.0;
    printf("Кодирование первых %d символов (Хаффман):\n", nfirst);
    printf("Битовая последовательность:\n%s\n\n", bits);
    printf("Длина закодированной последовательности: %lld бит\n", enc_bits);
    printf("Длина исходной последовательности:      %lld бит\n", orig_bits);
    printf("Коэффициент сжатия (orig/encoded):      %.6f\n\n", cr);
    free(bits);
}

static void encode_first100_gm(Sym *a, int n, Map *map, int nmap,
                               uint32_t *first_cps, int nfirst, long long first_bytes)
{
    size_t cap = 8192, blen = 0;
    char *bits = (char *)malloc(cap);
    if (!bits)
    {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i < nfirst; i++)
    {
        int idx = map_bsearch_idx(map, nmap, first_cps[i]);
        if (idx < 0)
            continue;
        int L = a[idx].Lgm;
        if (blen + (size_t)L + 1 > cap)
        {
            while (blen + (size_t)L + 1 > cap)
                cap *= 2;
            bits = (char *)realloc(bits, cap);
            if (!bits)
            {
                perror("realloc");
                exit(1);
            }
        }
        memcpy(bits + blen, a[idx].code_gm, (size_t)L);
        blen += (size_t)L;
    }
    bits[blen] = '\0';
    long long enc_bits = (long long)blen;
    long long orig_bits = (long long)first_bytes * 8LL;
    double cr = enc_bits ? (double)orig_bits / (double)enc_bits : 0.0;
    printf("Кодирование первых %d символов (Гилберт–Мур):\n", nfirst);
    printf("Битовая последовательность:\n%s\n\n", bits);
    printf("Длина закодированной последовательности: %lld бит\n", enc_bits);
    printf("Длина исходной последовательности:      %lld бит\n", orig_bits);
    printf("Коэффициент сжатия (orig/encoded):      %.6f\n\n", cr);
    free(bits);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Использование: %s <файл UTF-8>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f)
    {
        fprintf(stderr, "Не удалось открыть '%s': %s\n", argv[1], strerror(errno));
        return 1;
    }

    Vec v;
    vec_init(&v);
    long long total_syms = 0, total_bytes = 0;
    unsigned char buf[1 << 15];
    size_t r;

    uint32_t first_cps[100];
    int nfirst = 0;
    long long first_bytes = 0;

    while ((r = fread(buf, 1, sizeof(buf), f)) > 0)
    {
        total_bytes += (long long)r;
        size_t i = 0;
        while (i < r)
        {
            uint32_t cp;
            size_t got;
            if (!utf8_decode_one(buf + i, r - i, &cp, &got))
            {
                cp = (uint32_t)buf[i];
                got = 1;
            }
            if (nfirst < 100)
            {
                first_cps[nfirst++] = cp;
                first_bytes += (long long)got;
            }
            i += got;
            Sym *s = vec_find(&v, cp);
            if (!s)
            {
                Sym t;
                t.cp = cp;
                t.f = 1;
                t.p = 0.0;
                t.q = 0.0;
                t.Lsh = 0;
                t.Lf_med = 0;
                t.Lf_a2 = 0;
                t.Lh = 0;
                t.Lgm = 0;
                t.code_sh[0] = 0;
                t.code_f_med[0] = 0;
                t.code_f_a2[0] = 0;
                t.code_h[0] = 0;
                t.code_gm[0] = 0;
                vec_push(&v, t);
            }
            else
                s->f++;
            total_syms++;
        }
    }
    fclose(f);

    if (total_syms == 0)
    {
        fprintf(stderr, "Файл пуст\n");
        free(v.a);
        return 1;
    }

    for (int i = 0; i < v.n; i++)
        v.a[i].p = (double)v.a[i].f / (double)total_syms;

    // сортировка по частоте для Шеннона/Фано/Хаффмана
    qsort(v.a, v.n, sizeof(Sym), cmp_freq_desc);

    build_shannon(v.a, v.n);
    build_fano_med(v.a, v.n);
    build_fano_a2_all(v.a, v.n);
    build_huffman(v.a, v.n);
    build_gilbert_moore(v.a, v.n);

    print_table_shannon(argv[1], total_bytes, total_syms, v.a, v.n);
    print_metrics_shannon(v.a, v.n);

    print_table_fano_med(argv[1], v.a, v.n);
    print_metrics_fano_med(v.a, v.n);

    print_table_fano_a2(argv[1], v.a, v.n);
    print_metrics_fano_a2(v.a, v.n);

    print_table_huffman(argv[1], v.a, v.n);
    print_metrics_huffman(v.a, v.n);

    print_table_gm(argv[1], v.a, v.n);
    print_metrics_gm(v.a, v.n);

    print_metrics();
    print_metrics_shannon_table(v.a, v.n);
    print_metrics_fano_med_table(v.a, v.n);
    print_metrics_fano_a2_table(v.a, v.n);
    print_metrics_huffman_table(v.a, v.n);
    print_metrics_gm_table(v.a, v.n);

    Map *map = (Map *)malloc(v.n * sizeof(Map));
    if (!map)
    {
        perror("malloc");
        free(v.a);
        return 1;
    }
    for (int i = 0; i < v.n; i++)
    {
        map[i].cp = v.a[i].cp;
        map[i].idx = i;
    }
    qsort(map, v.n, sizeof(Map), cmp_map_cp);

    encode_first100_shannon(v.a, v.n, map, v.n, first_cps, nfirst, first_bytes);
    encode_first100_fano_med(v.a, v.n, map, v.n, first_cps, nfirst, first_bytes);
    encode_first100_fano_a2(v.a, v.n, map, v.n, first_cps, nfirst, first_bytes);
    encode_first100_huffman(v.a, v.n, map, v.n, first_cps, nfirst, first_bytes);
    encode_first100_gm(v.a, v.n, map, v.n, first_cps, nfirst, first_bytes);

    free(map);
    free(v.a);
    return 0;
}
