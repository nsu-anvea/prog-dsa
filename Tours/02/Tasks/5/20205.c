#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 9785021
#define prime 31

int N, B;

long int multiplier;

long int hash_t;
int      color_t;

typedef struct subStr {
    int color;
    int subStr;
    struct subStr * next;
} SubStr;

typedef struct list {
    struct subStr * first;
} List;

void push(List * G, SubStr ** S, int i) {
    SubStr * A = (SubStr *)malloc(sizeof(SubStr));
    A->subStr = i;
    A->color = color_t;
    A->next = NULL;

    if (* S == NULL) G->first = A;
    else (* S)->next = A;
}

List * create() {
    List * S = (List *)malloc(sizeof(List) * size);
    for (int i = 0; i < size; i++) {
        S[i].first = NULL;
    }
    return S;
}

char * getStr() {
    char * S = (char *)malloc(sizeof(char) * N + 1);
    scanf("%s", S);

    return S;
}

int absolute(int a) {
    if (a < 0) return -1 * a;
    return a;
}

long int hash(long int a, long int b) {
    return (hash_t - a * multiplier) * prime + b;
}

void execute() {
    char * str = getStr();

    // Creating hashTable & hash
    List* hashTable = create();
    for (int i = B-1; i >0; i--) {
        hash_t = (hash_t  + ((int)str[i]) * multiplier);
        multiplier *= prime;
    }
    hash_t = hash_t + (int)str[0] * multiplier;

    // Fill hashTable
    SubStr * t, * p;
    int isWrote, isDub, index;
    for (int x = 0; x < (N - B + 1); x++) {
        index = absolute(hash_t % size);
        t = hashTable[index].first;
        p = NULL;
        isWrote = 0;
        while (t) {
            isDub = 1;
            for (int i = 0; i < B; i++) {
                if (str[x + i] != str[t->subStr + i]) {
                    isDub = 0;
                    break;
                }
            }
            if (isDub) {
                isWrote = 1;
                printf("%d ", t->color);
                break;
            }
            p = t;
            t = t->next;
        }
        if (!isWrote) {
            push(&hashTable[index], &p, x);
            printf("%d ", color_t);
            color_t++;
        }
        hash_t = hash((long int)str[x], (long int)str[(x + B) % N]);
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d %d\n", &N, &B);

    color_t = 0;
    hash_t = 0;
    multiplier = 1;

    execute();

    return 0;
}