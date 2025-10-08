#include <stdio.h>
#include <stdlib.h>
#define size 1000000
#define maxSize 1000000000

typedef struct list {
    int             a;
    struct list *   next;
} Element;

typedef struct List {
    struct list *   top;
} List;

void push(List * L, Element * S, int e) {
    Element * A = (Element *)malloc(sizeof(Element));
    A->a = e;
    A->next = NULL;
    if (S != NULL) S->next = A;
    else {
        Element * p = L->top;
        L->top = A;
        free(p);
    }
}

List * createHashTable() {
    List * S = (List *)malloc(sizeof(List) * size);
    for (int i = 0; i < size; i++) {
        S[i].top = NULL;
    }
    return S;
}

int * createAnswer() {
    int * S = (int *)malloc(sizeof(int) * size);
    return S;
}

void emptyHashTable(List * ht) {
    Element * t, * p;
    for (int i = 0; i < size; i++) {
        t = ht[i].top;
        if (t != NULL) {
            while (t != NULL) {
                p = t;
                t = t->next;
                free(p);
            }
        }
    }
    free(ht);
}

void conclusion(int * q, int l, FILE * out) {
    fwrite(&l, sizeof(int), 1, out);
    for (int i = 0; i < l; i++) {
        fwrite(&q[i], sizeof(int), 1, out);
    }
}

int hash(int a) {
    return (a + maxSize) % size;
}

void addHashTable(int x, List * ht, int * q, int * len) {
    int index = hash(x);

    Element * t = ht[index].top;
    Element * p = NULL;
    int flag = 0;
    while (t != NULL) {
        if (x == t->a) {
            flag = 1;
            break;
        }
        p = t;
        t = t->next;
    }
    if (!flag) {
        push(&ht[index], p, x);
        q[*len] = x;
        *len += 1;
    }
}

void execute(FILE * in, FILE * out) {
    int N;
    fread(&N, sizeof(int), 1, in);

    List * hashTable = createHashTable();
    int * answer = createAnswer();
    int lenAnswer = 0;

    int x;
    for (int i = 0; i < N; i++) {
        fread(&x, sizeof(int), 1, in);
        addHashTable(x, hashTable, answer, &lenAnswer);
    }
    conclusion(answer, lenAnswer, out);
    emptyHashTable(hashTable);
    free(answer);
}

int main() {
    FILE * input = fopen("input.bin", "rb");
    FILE * output = fopen("output.bin", "wb");

    execute(input, output);

    return 0;
}