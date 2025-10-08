#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct encoding {
    int             len;
    int             size;
    unsigned char  *code;
} Encoding;

typedef struct node {
    int           frequency;
    unsigned char symbol;
    struct node  *left;
    struct node  *right;

    struct node  *next;
} Node;

typedef struct list {
    struct node  *first;
} List;

typedef struct heapItem {
    int           frequency;
    unsigned char symbol;
} HeapItem;

typedef struct heap {
    int       len;
    HeapItem *set;
} Heap;

void swap(HeapItem *a, HeapItem *b) {
    HeapItem c = *a;
    *a = *b;
    *b = c;
}

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return (2 * i + 1);
}

int right(int i) {
    return (2 * i + 2);
}

HeapItem *give(Heap *heap, int i) {
    return &heap->set[i];
}

int compare(Heap *heap, int i, int j) {
    return give(heap, i)->frequency < give(heap, j)->frequency ||
           (
                   give(heap, i)->frequency == give(heap, j)->frequency &&
                   give(heap, i)->symbol > give(heap, j)->symbol
           );
}

void sift_up(Heap *heap, int i) {
    if (!i) return;
    int p = parent(i);
    if (compare(heap, i, p)) {
        swap(give(heap, i), give(heap, p));
        sift_up(heap, p);
    }
}

void sift_down(Heap *heap, int i) {
    int l = left(i);
    int r = right(i);
    int t = i;
    if (l < heap->len && compare(heap, l, i))
        t = l;
    if (r < heap->len && compare(heap, r, t))
        t = r;
    if (t != i) {
        swap(give(heap, i), give(heap, t));
        sift_down(heap, t);
    }
}

void push(Heap *heap, int frequency, unsigned char symbol) {
    give(heap, heap->len)->frequency = frequency;
    give(heap, heap->len)->symbol = symbol;
    sift_up(heap, heap->len);
    heap->len++;
}

void heap_sort(Heap *heap) {
    int len = heap->len;
    for (int i = len - 1; i > 0; i--) {
        swap(give(heap, i), give(heap, 0));
        heap->len--;
        sift_down(heap, 0);
    }
    heap->len = len;
}

Heap *create_heap(size_t size) {
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap->len = 0;
    heap->set = (HeapItem *)malloc(sizeof(HeapItem) * size);
    return heap;
}

List *create_nodes() {
    List *nodes = (List *)malloc(sizeof(List));
    nodes->first = NULL;
    return nodes;
}

Encoding *create_encodings(size_t size) {
    Encoding *encodings = (Encoding *)malloc(sizeof(Encoding) * size);
    for (int i = 0; i < size; i++) {
        encodings[i].len = 0;
        encodings[i].size = 8;
        encodings[i].code = (unsigned char *)calloc(8, sizeof(unsigned char));
    }
    return encodings;
}

void fill_heap(Heap *heap, int *hist) {
    for (int i = 0; i < 256; i++)
        if (hist[i])
            push(heap, hist[i], (unsigned char)i);
}

void fill_nodes(List *nodes, Heap *heap) {
    for (int i = 0; i < heap->len; i++) {
        Node *a = (Node *)malloc(sizeof(Node));
        a->frequency = give(heap, i)->frequency;
        a->symbol = give(heap, i)->symbol;
        a->left = NULL;
        a->right = NULL;

        if (!nodes->first) {
            nodes->first = a;
            a->next = NULL;
        }
        else {
            a->next = nodes->first;
            nodes->first = a;
        }
    }
}

void insert(List *nodes, Node *a) {
    Node *c = nodes->first;
    Node *p = NULL;
    while (c && (c->frequency <= a->frequency)) {
        p = c;
        c = p->next;
    }
    if (!p) {
        a->next = c;
        nodes->first = a;
    }
    else if (!c) {
        p->next = a;
    }
    else {
        p->next = a;
        a->next = c;
    }
}

Node *build_Huffman_tree(List *nodes, int len) {
    while (len - 1) {
        Node *f = nodes->first;
        Node *s = nodes->first->next;

        nodes->first = s->next;

        f->next = NULL;
        s->next = NULL;

        Node *a = (Node *)malloc(sizeof(Node));
        a->frequency = f->frequency + s->frequency;
        a->symbol = 0;
        a->left = s;
        a->right = f;

        a->next = NULL;

        len--;

        insert(nodes, a);
    }
    return nodes->first;
}

int check_leaf(Node *T) {
    if (!T->left && !T->right)
        return 1;
    return 0;
}

long long int get_encoded_file_len(Node *T, Encoding *E, int *hist, Encoding *buff, int *nodes_count, int len) {
    if (check_leaf(T)) {
        (*nodes_count)++;

        if (E[T->symbol].size < buff->size) {
            E[T->symbol].size = buff->size;
            E[T->symbol].code = (unsigned char *) realloc(E[T->symbol].code, sizeof(unsigned char) * buff->size);
        }
        E[T->symbol].len = len;
        for (int i = 0; i < ceil((float)len / 8); i++)
            E[T->symbol].code[i] = buff->code[i];
        return (long long int)len * hist[T->symbol];
    }
    if ((len / 8 + 2) == buff->size) {
        buff->size *= 2;
        buff->code = (unsigned char *) realloc(buff->code, sizeof(unsigned char) * buff->size);
    }
    (*nodes_count)++;

    buff->code[len / 8] &= ((unsigned char)255 - ((unsigned char)1 << (7 - (len % 8))));
    long long int depthLeft = get_encoded_file_len(T->left, E, hist, buff, nodes_count, len + 1);

    buff->code[len / 8] |= (unsigned char)1 << (7 - (len % 8));
    long long int depthRight = get_encoded_file_len(T->right, E, hist, buff, nodes_count, len + 1);
    return depthLeft + depthRight;
}

void write_byte(unsigned char *byte, int *byte_len, FILE *out) {
    if (* byte_len == 8) {
        fwrite(byte, sizeof(unsigned char), 1, out);
        * byte_len = 0;
        * byte = 0;
    }
}

void write_tree(Node *T, unsigned char *byte, int *byte_len, FILE *out) {
    if (check_leaf(T)) {
        * byte |= (unsigned char)1 << (7 - *byte_len);
        (* byte_len)++;
        write_byte(byte, byte_len, out);
        for (int i = 0; i < 8; i++) {
            if (T->symbol & ((unsigned char)1 << (7 - i))) {
                * byte |= (unsigned char) 1 << (7 - *byte_len);
                (* byte_len)++;
                write_byte(byte, byte_len, out);
            }
            else {
                (* byte_len)++;
                write_byte(byte, byte_len, out);
            }
        }
        return;
    }
    (* byte_len)++;
    write_byte(byte, byte_len, out);
    write_tree(T->left, byte, byte_len, out);
    write_tree(T->right, byte, byte_len, out);
}

void clear_tree(Node *T) {
    if (!T)
        return;
    if (T->left)
        clear_tree(T->left);
    if (T->right)
        clear_tree(T->right);
    free(T);
}

void encode(int argc, char **argv, FILE *out) {
    int original_size = 0;

    int hist[256] = {0};

    Heap *heap  = create_heap(256);
    List *nodes = create_nodes();

    Encoding *encodings = create_encodings(256);
    Encoding *buff      = create_encodings(1);

    int files_count = argc - 3;
    long long int bytes_occupied = 12 + files_count * 4;
    fwrite(&files_count,    sizeof(int), 1, out);
    fwrite(&bytes_occupied, sizeof(long long int), 1, out);

    for (int file = 3; file < argc; file++) {
        FILE *in = fopen(argv[file], "rb");

        int encoding_len = 5;

        unsigned char name_len = strlen(argv[file]);
        encoding_len += name_len;

        int file_size = 0;

        unsigned char *data_in = (unsigned char *) malloc(sizeof(unsigned char) * 100);
        int size_data_in = 100;

        unsigned char symbol;
        while (fscanf(in, "%c", &symbol) != EOF) {
            hist[symbol]++;

            data_in[file_size++] = symbol;
            if (file_size == size_data_in) {
                size_data_in *= 2;
                data_in = (unsigned char *) realloc(data_in, sizeof(unsigned char) * size_data_in);
            }
        }
        original_size += file_size;

        fill_heap(heap, hist);
        heap_sort(heap);
        encoding_len += heap->len;

        fill_nodes(nodes, heap);

        Node *Huffman_tree = build_Huffman_tree(nodes, heap->len);

        int nodes_count = 0;
        long long int encoded_file_len = get_encoded_file_len(
                Huffman_tree, encodings, hist, buff, &nodes_count, 0);
        encoding_len += ceil(((double)encoded_file_len) / 8);
        encoding_len += ceil(((double)nodes_count) / 8);

        bytes_occupied += encoding_len;

        /* Filling in the archive */
        unsigned char byte = 0;
        int byte_len = 0;

        fwrite(&encoding_len, sizeof(int), 1, out);

        fwrite(&name_len, sizeof(unsigned char), 1, out);
        for (int i = 0; i < name_len; i++)
            fwrite(&argv[file][i], sizeof(unsigned char), 1, out);

        fwrite(&file_size, sizeof(int), 1, out);

        write_tree(Huffman_tree, &byte, &byte_len, out);
        if (byte_len)
            fwrite(&byte, sizeof(unsigned char), 1, out);

        byte = 0;
        byte_len = 0;

        for (int i = 0; i < file_size; i++) {
            symbol = data_in[i];
            for (int j = 0; j < encodings[symbol].len; j++) {
                if (encodings[symbol].code[j / 8] & ((unsigned char) 1 << (7 - (j % 8)))) {
                    byte |= (unsigned char) 1 << (7 - byte_len);
                }
                byte_len++;
                write_byte(&byte, &byte_len, out);
            }
        }
        if (byte_len)
            fwrite(&byte, sizeof(unsigned char), 1, out);

        /* Reset */
        clear_tree(Huffman_tree);

        for (int i = 0; i < 256; i++) {
            hist[i] = 0;

            for (int j = 0; j < encodings[i].size; j++) {
                encodings[i].code[j] = 0;
            }
            encodings[i].len = 0;
            encodings[i].size = 0;
        }
        for (int i = 0; i < 256; i++) {
            buff->code[i] = 0;
        }
        buff->len  = 0;
        buff->size = 0;
        heap->len  = 0;
        nodes->first = NULL;

        fclose(in);
    }

    fseek(out, 4, SEEK_SET);
    fwrite(&bytes_occupied, sizeof(long long int), 1, out);

    printf("\n... <zipa>: Сompressed to %.3f%%\n", ((float)bytes_occupied / original_size) * 100);

    /* Clearning the memory */
    free(heap->set);
    free(heap);
    for (int i = 0; i < 256; i++)
        free(encodings[i].code);
    free(encodings);
    free(buff->code);
    free(buff);
}

void read_byte(unsigned char * byte, int * len, FILE *in) {
    if (*len == 8) {
        fscanf(in, "%c", byte);
        *len = 0;
    }
}

Node * recover_Huffman_tree(Node * T, unsigned char * byte, int * byte_len, FILE *in) {
    T = (Node *)malloc(sizeof(Node));
    T->symbol    = 0;
    T->frequency = 0;
    T->left      = NULL;
    T->right     = NULL;
    T->next      = NULL;

    if ((* byte) & (unsigned char)1 << (7 - (* byte_len))) {
        (* byte_len)++;
        read_byte(byte, byte_len, in);
        for (int i = 0; i < 8; i++) {
            if ((* byte) & (unsigned char)1 << (7 - (* byte_len))) {
                T->symbol |= (unsigned char)1 << (7 - i);
            }
            (* byte_len)++;
            read_byte(byte, byte_len, in);
        }
    }
    else {
        (* byte_len)++;
        read_byte(byte, byte_len, in);
        T->left  = recover_Huffman_tree(T->left, byte, byte_len, in);
        T->right = recover_Huffman_tree(T->left, byte, byte_len, in);
    }
    return T;
}

void decode(int depth, Node *T, int *file_size, unsigned char *byte, int *len, FILE *in, FILE *out) {
    if (check_leaf(T)) {
        fprintf(out, "%c", T->symbol);
        (* file_size)--;
        return;
    }
    if ((* byte) & (unsigned char)1 << (7 - (* len))) {
        (* len)++;
        read_byte(byte, len, in);
        decode(depth + 1, T->right, file_size, byte, len, in, out);
    }
    else {
        (* len)++;
        read_byte(byte, len, in);
        decode(depth + 1, T->left, file_size, byte, len, in, out);
    }
    while (!depth && (* file_size)) {
        decode(depth + 1, T, file_size, byte, len, in, out);
    }
}

int check_name(unsigned char len, char *name_in_arc, char *name_file) {
    for (int i = 0; i < len; i++) {
        if (name_file[i] != name_in_arc[i])
            return 0;
    }
    return 1;
}

void extract(int argc, char **argv, FILE *in) {
    int files_count;
    long long int bytes_occupied;
    fread(&files_count,    sizeof(int), 1, in);
    fread(&bytes_occupied, sizeof(long long int), 1, in);

    int encoding_len;

    unsigned char name_file_len;
    unsigned char name_in_arc_len;

    char *name_in_arc = (char *) malloc(sizeof(char) * 256);

    int files_extracted = 0;
    while (fread(&encoding_len, sizeof(int), 1, in) == 1) {
        fscanf(in, "%c", &name_in_arc_len);
        fread(name_in_arc, sizeof(unsigned char), name_in_arc_len, in);

        int flag_extract_file = 0;
        for (int file = 3; file < argc; file++) {
            name_file_len = strlen(argv[file]);

            if (name_file_len == name_in_arc_len) {
                if (check_name(name_file_len, name_in_arc, argv[file])) {
                    flag_extract_file = 1;

                    FILE *out = fopen(argv[file], "wb");

                    int file_size;
                    fread(&file_size, sizeof(int), 1, in);

                    unsigned char byte;
                    int byte_len = 0;

                    fscanf(in, "%c", &byte);
                    Node *Huffman_tree = NULL;
                    Huffman_tree = recover_Huffman_tree(Huffman_tree, &byte, &byte_len, in);

                    fscanf(in, "%c", &byte);
                    byte_len = 0;
                    decode(0, Huffman_tree, &file_size, &byte, &byte_len, in, out);

                    clear_tree(Huffman_tree);

                    files_extracted++;

                    fclose(out);
                    break;
                }
            }
        }
        if (!flag_extract_file)
            fseek(in, encoding_len - (name_in_arc_len + 1), SEEK_CUR);
    }
    printf("\n... <zipa>: Files extracted: %d\n\n", files_extracted);
    free(name_in_arc);
}

void delete(int argc, char **argv, FILE *in) {
    FILE *out = fopen("arc_temp.bin", "wb");

    int files_count;
    long long int bytes_occupied;
    fread(&files_count,    sizeof(int), 1, in);
    fread(&bytes_occupied, sizeof(long long int), 1, in);

    int files_count_temp = files_count;
    long long int bytes_occupied_temp = bytes_occupied;
    fwrite(&files_count,    sizeof(int), 1, out);
    fwrite(&bytes_occupied, sizeof(long long int), 1, out);

    int encoding_len;
    
    unsigned char name_file_len;
    unsigned char name_in_arc_len;

    char *name_in_arc = (char *) malloc(sizeof(char) * 256);
    
    while (fread(&encoding_len, sizeof(int), 1, in) == 1) {
        fscanf(in, "%c", &name_in_arc_len);
        fread(name_in_arc, sizeof(unsigned char), name_in_arc_len, in);

        int flag_del_file = 0;
        for (int file = 3; file < argc; file++) {
            name_file_len = strlen(argv[file]);

            if (name_file_len == name_in_arc_len) {
                if (check_name(name_file_len, name_in_arc, argv[file]))
                    flag_del_file = 1;
            }
        }
        if (flag_del_file) {
            files_count_temp--;
            bytes_occupied_temp -= (encoding_len + 4);
            fseek(in, encoding_len - (name_in_arc_len + 1), SEEK_CUR);
        }
        else {
            fwrite(&encoding_len, sizeof(int), 1, out);

            fwrite(&name_in_arc_len, sizeof(unsigned char), 1, out);
            for (int i = 0; i < name_in_arc_len; i++) {
                fwrite(&name_in_arc[i], sizeof(unsigned char), 1, out);
            }
            unsigned char byte;
            for (int i = 0; i < encoding_len - (name_in_arc_len + 1); i++) {
                fscanf(in, "%c", &byte);
                fwrite(&byte, sizeof(unsigned char), 1, out);
            }
        }
    }

    if (files_count_temp != files_count) {
        fseek(out, 0, SEEK_SET);
        fwrite(&files_count_temp,    sizeof(int), 1, out);
        fwrite(&bytes_occupied_temp, sizeof(long long int), 1, out);
    }
    else {
        printf("\nError: a file with that name was not found\n");
    }
    printf("\n... <zipa>: Files have been deleted:   %d\n", files_count - files_count_temp);
    printf("... <zipa>: Bytes have been deleted:   %lld\n\n", bytes_occupied - bytes_occupied_temp);

    fclose(out);

    remove("arc.bin");
    rename("arc_temp.bin", "arc.bin");
    free(name_in_arc);
}

void help() {
    printf("\n\n\n\n\n\t\t\tGeneral Commands Manual\n");

    printf("NAME\n");
    printf("\tzipa - an archiver made by Andrew\n\tduring his studies at Novosibirsk State University\n\n");

    printf("SYNOPSIS\n");
    printf("\t./zipa [-a | -x | -d | -i | -h] arc.bin file(s)[.ext]\n\n");

    printf("DESCRIPTION\n");
    printf("\tThe zipa utility archives using the Huffman method,\n\textracts or deletes the file(s)\n\n");
    printf("\tThe options are as follows:\n\n");
    printf("\t-a\tarchive the file(s)\n\n");
    printf("\t-x\textract the file(s)\n\n");
    printf("\t-d\tdelete the file(s) from the archive\n\n");
    printf("\t-i\tget information about the archive\n\n");
    printf("\t-h\tget help on how the utility works\n\n");
}

void info(FILE *in) {
    int files_count;
    long long int bytes_occupied;
    fread(&files_count, sizeof(int), 1, in);
    fread(&bytes_occupied, sizeof(long long int), 1, in);

    printf("\nFiles: %d\n",   files_count);
    printf("Bytes: %lld\n\n", bytes_occupied);

    printf("List of file names:\n");

    int encoding_len;
    for (int i = 0; i < files_count; i++) {
        printf("\t");

        fread(&encoding_len, sizeof(int), 1, in);

        unsigned char file_name_len;
        fscanf(in, "%c", &file_name_len);

        unsigned char symbol;
        for (int j = 0; j < file_name_len; j++) {
            fscanf(in, "%c", &symbol);
            printf("%c", symbol);
        }
        printf("\t%d\n", encoding_len + 4);
        fseek(in, encoding_len - (file_name_len + 1), SEEK_CUR);
    }
}

int main(int argc, char **argv) {
    clock_t start = clock();

    if (argc == 1) {
        help();
        return 0;
    }
    if (argv[1][0] != '-') {
        printf("\nError 1: incorrect usage utility!\n");
        printf("To see the usage hint, you need to write \"./zipa -h\"\n\n");
        return 1;
    }
    char cmd = argv[1][1];

    if (argc == 2 && cmd == 'h') {
        help();
        return 0;
    }
    else {
        if (argc < 4 && cmd != 'i') {
            printf("\nError 2: there are too few arguments!\n\n");
            return 1;
        }
    }

    if (cmd == 'a') {
        FILE *arc = fopen(argv[2], "wb");
        encode(argc, argv, arc);
        fclose(arc);
    }
    else {
        FILE *arc = fopen(argv[2], "rb");

        if (cmd == 'x')
            extract(argc, argv, arc);
        else if (cmd == 'd')
            delete(argc, argv, arc);
        else if (cmd == 'i')
            info(arc);

        fclose(arc);
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTIME: %f seconds\n\n", time_spent);

    return 0;
}