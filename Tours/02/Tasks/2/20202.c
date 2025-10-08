#include <stdio.h>
#include <stdlib.h>
#define size 9785021

unsigned long long int M;
unsigned long long int ae, be, ce;

typedef struct list {
    unsigned long long int a;
    unsigned long long int i;
    struct list * next;
} Element;

typedef struct queue {
    unsigned long long int len;
    struct list * first;
    struct list * end;
} Queue;

void enQueue(Queue * S, unsigned long long int x, unsigned long long int ind) {
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

unsigned long long int deQueue(Queue * S) {
    Element * Del = S->first;
    S->first = Del->next;
    S->len--;

    unsigned long long int a = Del->a;

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
    unsigned long long int lenght;
    for (int i = 0; i < size; i++) {
        lenght = S[i].len;
        for (unsigned long long int j = 0; j < lenght; j++) {
            deQueue(&S[i]);
        }
    }
    free(S);
}

void conclusion(unsigned long long int l, unsigned long long int r) {
    printf("%llu %llu", l, r);
}

unsigned long long int func(unsigned long long int x) {
    return (x * x * ae + x * be + ce) % M;
}

unsigned long long int hash(unsigned long long int x) {
    return x % size;
}

void execute() {
    Queue * hashTable = createHashTable();
    enQueue(&hashTable[hash(1)], 1, 0);
    
    unsigned long long int l;
    unsigned long long int r;
    
    unsigned long long int state = func(1);
    unsigned long long int iHash = hash(state);
    unsigned long long int position = 1;
    
    Element * t;
    int flag = 0;
    while (1) {
        t = hashTable[iHash].first;
        while (t != NULL) {
            if (state == t->a) {
                l = t->i;
                r = position;
                flag = 1;
                break;
            }
            t = t->next;
        }
        if (flag == 1) {
            break;
        }
        enQueue(&hashTable[iHash], state, position);
        
        state = func(state);
        iHash = hash(state);
        position++;
    }
    conclusion(l, r);
    empty(hashTable);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%llu\n", &M);
    scanf("%llu %llu %llu", &ae, &be, &ce);

    execute();

    return 0;
}