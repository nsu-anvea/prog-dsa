#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
    int             a;
    struct list *   next;
    struct list *   prev;
} list;

typedef struct deq {
    int             len;
    struct list *   top;
    struct list *   end;
} Deq;

void enDeqtop(Deq * S, int e) {
    list * A = (list *)malloc(sizeof(list));
    A->a = e;

    if (S->top == NULL) {
        A->next = NULL;
        A->prev = NULL;
        S->top = A;
        S->end = A;
    } else {
        A->next = S->top;
        A->prev = NULL;
        S->top->prev = A;
        S->top = A;
    }
    S->len++;
}

void enDeqend(Deq * S, int e) {
    list * A = (list *)malloc(sizeof(list));
    A->a = e;

    if (S->top == NULL) {
        A->next = NULL;
        A->prev = NULL;
        S->top = A;
        S->end = A;
    } else {
        A->next = NULL;
        A->prev = S->end;
        S->end->next = A;
        S->end = A;
    }
    S->len++;
}

int deDeqtop(Deq * S) {
    list * A = S->top;
    if (S->len > 1) {
        A->next->prev = NULL;
        S->top = A->next;
    }
    else if (S->len == 1) {
        S->top = NULL;
        S->end = NULL;
    }
    S->len--;

    int a = A->a;

    free(A);
    return a;
}

int deDeqend(Deq * S) {
    list * A = S->end;
    if (S->len > 1) {
        A->prev->next = NULL;
        S->end = A->prev;
    }
    else if (S->len != 0) {
        S->top = NULL;
        S->end = NULL;
    }
    S->len--;

    int a = A->a;

    free(A);
    return a;
}

Deq * createDeq(int a, int step) {
    Deq * S = (Deq *)malloc(sizeof(Deq));
    S->len = 0;
    S->top = NULL;
    S->end = NULL;
    
    for (int i = 1; i <= a; i++) {
        enDeqend(S, i * step);
    }
    return S;
}

void conclusion(Deq * L, Deq * R) {
    int l = L->len, r = R->len;

    printf("%d ", l);
    for (int i = 0; i < l; i++) {
        printf("%d ", deDeqtop(L));
    }

    printf("\n%d ", r);
    for (int i = 0; i < r; i++) {
        printf("%d ", deDeqtop(R));
    }
    free(L);
    free(R);
}

int check(int cmd, int shift) { //shift - смещение
    int mask = 1 << shift;
    return cmd & mask;
}

void execute(int cmd, Deq * L, Deq * R) {
    // Take the need structure
    int Take, flag = 0;
    if (check(cmd, 3) && R->len != 0) {    // Take from R deck
        if (check(cmd, 2)) {               // Take from end
            if (R->end != NULL) {
                Take = R->end->a;
                flag = 1;
                deDeqend(R);
            }
        }
        else {                             // Take from top
            if (R->top != NULL) {
                Take = R->top->a;
                flag = 1;
                deDeqtop(R);
            }
        }
    }
    if (!check(cmd,3) && L->len != 0) {    // Take from L deck
        if (check(cmd, 2)) {               // Take from end
            if (L->end != NULL) {
                Take = L->end->a;
                flag = 1;
                deDeqend(L);
            }
        }
        else {                             // Take from top
            if (L->top != NULL) {
                Take = L->top->a;
                flag = 1;
                deDeqtop(L);
            }
        }
    }
    // Put the taken structure
    if (flag) {
        if (check(cmd, 1)) {        // Put (* Take) in R deck
            if (check(cmd, 0)) {    // Put (* Take) down
                enDeqend(R, Take);
            } else {                // Put (* Take) up
                enDeqtop(R, Take);
            }
        } else {                    // Put (* Take) in L deck
            if (check(cmd, 0)) {    // Put (* Take) down
                enDeqend(L, Take);
            } else {                // Put (* Take) up
                enDeqtop(L, Take);
            }
        }
    }
}

int defineNumber(char command) {
    int cmd;
    if (command > '9') {
        cmd = (int)command - 'A' + 10;
    } else {
        cmd = (int)command - '0';
    }
    return cmd;
}

void cognates(int M, Deq * L, Deq * R) {
    char command;
    for (int i = 0; i < M; i++) {
        scanf("%c", &command);

        execute(defineNumber(command), L, R);
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N, M;
    scanf("%d %d\n", &N, &M);

    Deq * deckLeft = createDeq(N, 1);   // from 1 to N, step = 1
    Deq * deckRight = createDeq(N, -1); // from -1 to -N, step = -1

    cognates(M, deckLeft, deckRight);   // тасовка

    conclusion(deckLeft, deckRight);

    return 0;
}