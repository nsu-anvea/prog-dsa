#include <stdio.h>
#include <stdlib.h>
#define size 9785021

unsigned int M;
unsigned int ae, be, ce;

typedef struct list {
    unsigned int a;
    unsigned int i;
    struct list * next;
} Element;

typedef struct queue {
    unsigned int len;
    struct list * first;
    struct list * end;
} Queue;

void enQueue(Queue * S, unsigned int x, unsigned int ind) {
    Element * Add = (Element *)malloc(sizeof(Element));
    Add->a = x;
    Add->i = ind;
    Add->next = NULL;

    if (!S->len) {
        S->first = Add;
        S->end = Add;
    } else {
        S->end->next = Add;
        S->end = Add;
    }
    S->len++;
}

unsigned int deQueue(Queue * S) {
    Element * Del = S->first;
    S->first = Del->next;
    S->len--;

    unsigned int a = Del->a;

    free(Del);
    return a;
}

Queue * createHashTable() {
    Queue * S = (Queue *)malloc(sizeof(Queue) * size);

    for (int i = 0; i < size; i++) {
        S[i].len = 0;
        S[i].first = NULL;
        S[i].end = NULL;
    }
    return S;
}

void empty(Queue * S) {
    unsigned int lenght;
    for (int i = 0; i < size; i++) {
        lenght = S[i].len;
        for (unsigned int j = 0; j < lenght; j++) {
            deQueue(&S[i]);
        }
    }
    free(S);
}

void conclusion(unsigned int l, unsigned int r) {
    printf("answer %u %u\n", l, r);
    fflush(stdout);
}

unsigned int hash(unsigned int x) {
    return x % size;
}

void execute() {
    Queue * hashTable = createHashTable();
    
    unsigned int l;
    unsigned int r;
    
    unsigned int number;
    unsigned int hashNum;
    unsigned int key = 1;
    
    Element * t;
    int flag = 0;
    while (1) {
        printf("eval %u\n", key);
        fflush(stdout);

        scanf("%u", &number);
        hashNum = hash(number);

        t = hashTable[hashNum].first;
        while (t != NULL) {
            if (number == t->a) {
                l = t->i;
                r = key;
                flag = 1;
                break;
            }
            t = t->next;
        }
        if (flag == 1) {
            break;
        }
        enQueue(&hashTable[hashNum], number, key);
        
        key++;
    }
    conclusion(l, r);
    empty(hashTable);
}

int main() {

    execute();

    return 0;
}