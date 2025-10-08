#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct encoding {
    int             len;
    int             size;
    unsigned char * code;
} Encoding;

typedef struct node {
    int           frequency;
    unsigned char symbol;
    struct node * left;
    struct node * right;

    struct node * next;
} Node;

Encoding * createEncodings(size_t size) {
    Encoding * encodings = (Encoding *)malloc(sizeof(Encoding) * size);
    for (int i = 0; i < size; i++) {
        encodings[i].len = 0;
        encodings[i].size = 8;
        encodings[i].code = (unsigned char *)calloc(8, sizeof(unsigned char));
    }
    return encodings;
}

void isOverFlow(unsigned char * byte, int * len, FILE * in) {
    if (* len == 8) {
        fread(byte, sizeof(unsigned char), 1, in);
        * len = 0;
    }
}

Node * buildHuffmanTree(Node * T, unsigned char * byte, int * len, FILE * in) {
    T = (Node *)malloc(sizeof(Node));
    T->symbol    = 0;
    T->frequency = 0;
    T->left      = NULL;
    T->right     = NULL;
    T->next      = NULL;

    if ((* byte) & (unsigned char)1 << (7 - (* len))) {
        (* len)++;
        isOverFlow(byte, len, in);
        for (int i = 0; i < 8; i++) {
            if ((* byte) & (unsigned char)1 << (7 - (* len))) {
                T->symbol |= (unsigned char)1 << (7 - i);
            }
            (* len)++;
            isOverFlow(byte, len, in);
        }
    }
    else {
        (* len)++;
        isOverFlow(byte, len, in);
        T->left  = buildHuffmanTree(T->left, byte, len, in);
        T->right = buildHuffmanTree(T->left, byte, len, in);
    }
    return T;
}

int checkLeaf(Node * T) {
    if (!T->left && !T->right)
        return 1;
    return 0;
}

void getCodeLenght(Node * T, Encoding * E, Encoding * buff, int len) {
    if (checkLeaf(T)) {
        if (E[T->symbol].size < buff->size) {
            E[T->symbol].size = buff->size;
            E[T->symbol].code = (unsigned char *) realloc(E[T->symbol].code, sizeof(unsigned char) * buff->size);
        }
        if (!len) {
            E[T->symbol].len = 1;
        } else {
            E[T->symbol].len = len;
        }
        for (int i = 0; i < ceil((float)len / 8); i++)
            E[T->symbol].code[i] = buff->code[i];
        return;
    }
    if ((len / 8 + 2) == buff->size) {
        buff->size *= 2;
        buff->code = (unsigned char *) realloc(buff->code, sizeof(unsigned char) * buff->size);
    }

    buff->code[len / 8] &= ((unsigned char)255 - ((unsigned char)1 << (7 - (len % 8))));
    getCodeLenght(T->left, E, buff, len + 1);

    buff->code[len / 8] |= (unsigned char)1 << (7 - (len % 8));
    getCodeLenght(T->right, E, buff, len + 1);
}

void clear(Node * T) {
    if (!T)
        return;
    if (T->left)
        clear(T->left);
    if (T->right)
        clear(T->right);
    free(T);
}

void decode(int depth, Node * T, int * fileSize, unsigned char * byte, int * len, FILE * in, FILE * out) {
    if (checkLeaf(T)) {
        fprintf(out, "%c", T->symbol);
        (* fileSize)--;
        return;
    }
    if ((* byte) & (unsigned char)1 << (7 - (* len))) {
        (* len)++;
        isOverFlow(byte, len, in);
        decode(depth + 1, T->right, fileSize, byte, len, in, out);
    }
    else {
        (* len)++;
        isOverFlow(byte, len, in);
        decode(depth + 1, T->left, fileSize, byte, len, in, out);
    }
    while (!depth && (* fileSize)) {
        decode(depth + 1, T, fileSize, byte, len, in, out);
    }
}

int main() {
    FILE * in = fopen("input.bin", "rb");
    FILE * out = fopen("output.bin", "wb");

    int fileSize;
    fread(&fileSize, sizeof(int), 1, in);

    unsigned char symbols_count;
    fread(&symbols_count, sizeof(unsigned char), 1, in);

    unsigned char byte;
    int           len = 0;

    fread(&byte, sizeof(unsigned char), 1, in);
    Node * HuffmanTree = NULL;
    HuffmanTree = buildHuffmanTree(HuffmanTree, &byte, &len, in);

    Encoding * encodings = createEncodings(256);
    Encoding * buff = createEncodings(1);

    getCodeLenght(HuffmanTree, encodings, buff, 0);

    fread(&byte, sizeof(unsigned char), 1, in);
    len = 0;
    decode(0, HuffmanTree, &fileSize, &byte, &len, in, out);

    /* Clearning the memory */
    clear(HuffmanTree);
    for (int i = 0; i < 256; i++)
        free(encodings[i].code);
    free(encodings);
    free(buff->code);
    free(buff);

    return 0;
}