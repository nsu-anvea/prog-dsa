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

void isOverFlow(unsigned char * byte, int * len) {
    if (* len == 8) {
        fscanf(stdin, "%c", byte);
        * len = 0;
    }
}

Node * buildHuffmanTree(Node * T, unsigned char * sym_cnt, unsigned char * byte, int * len) {
    T = (Node *)malloc(sizeof(Node));
    T->symbol    = 0;
    T->frequency = 0;
    T->left      = NULL;
    T->right     = NULL;
    T->next      = NULL;
    
    if ((* byte) & (unsigned char)1 << (7 - (* len))) {
        (* len)++;
        isOverFlow(byte, len);
        for (int i = 0; i < 8; i++) {
            if ((* byte) & (unsigned char)1 << (7 - (* len))) {
                T->symbol |= (unsigned char)1 << (7 - i);
            }
            (* len)++;
            isOverFlow(byte, len);
        }
        (* sym_cnt)--;
        if (!sym_cnt)
            return T;
    }
    else {
        (* len)++;
        isOverFlow(byte, len);
        T->left = buildHuffmanTree(T->left, sym_cnt, byte, len);
        T->right = buildHuffmanTree(T->left, sym_cnt, byte, len);
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

int main() {
    freopen("input.bin", "rb", stdin);
    freopen("output.txt", "w", stdout);

    unsigned char symbols_count;
    fscanf(stdin, "%c", &symbols_count);

    unsigned char byte;
    int           len = 0;

    fscanf(stdin, "%c", &byte);
    Node * HuffmanTree = NULL;
    HuffmanTree = buildHuffmanTree(HuffmanTree, &symbols_count, &byte, &len);

    Encoding * encodings = createEncodings(256);
    Encoding * buff = createEncodings(1);

    getCodeLenght(HuffmanTree, encodings, buff, 0);

    for (int i = 0; i < 256; i++) {
        if (encodings[i].len) {
            printf("%d : ", i);
            for (int j = 0; j < encodings[i].len; j++) {
                if (encodings[i].code[j / 8] & ((unsigned char)1 << (7 - (j % 8))))
                    printf("1");
                else
                    printf("0");
            }
            printf("\n");
        }
    }

    /* Clearning the memory */
    clear(HuffmanTree);
    for (int i = 0; i < 256; i++)
        free(encodings[i].code);
    free(encodings);
    free(buff->code);
    free(buff);

    return 0;
}