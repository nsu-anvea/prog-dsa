#include <stdio.h>
#include <stdlib.h>

typedef struct deq {
    int     len;
    int     top;
    int     bottom;
    int *   deck;
} Deq;

int top(Deq * S) {
    return S->top;
}

int bottom(Deq * S) {
    return S->bottom;
}

void enDeqtop(Deq * S, int x, int n) {
    S->top = (top(S) + 2 * n) % (2 * n + 1);
    S->deck[top(S)] = x;
    S->len++;
}

void enDeqend(Deq * S, int x, int n) {
    if (!S->len) {
        enDeqtop(S, x, n);
    } else {
        S->deck[bottom(S)] = x;
        S->bottom = (bottom(S) + 1) % (2 * n + 1);
        S->len++;
    }
}

int deDeqtop(Deq * S, int n) {
    int a = S->deck[top(S)];
    S->deck[top(S)] = 0;
    S->top = (top(S) + 1) % (2 * n + 1);
    S->len--;
    return a;
}

int deDeqend(Deq * S, int n) {
    S->bottom = (bottom(S) + 2 * n) % (2 * n + 1);
    int a = S->deck[bottom(S)];
    S->deck[bottom(S)] = 0;
    S->len--;
    return a;
}

Deq * createDeq(int n, int step) {
    Deq * S = (Deq *)malloc(sizeof(Deq));
    S->len = 0;
    S->top = 0;
    S->bottom = 0;
    S->deck = (int *)malloc(sizeof(int) * (2 * n + 1));

    for (int i = 1; i <= n; i++) {
        enDeqend(S, i * step, n);
    }
    return S;
}

void conclusion(int n, Deq * L, Deq * R) {
    int l = L->len, r = R->len;

    printf("%d ", l);
    for (int i = 0; i < l; i++) {
        printf("%d ", deDeqtop(L, n));
    }

    printf("\n%d ", r);
    for (int i = 0; i < r; i++) {
        printf("%d ", deDeqtop(R, n));
    }
    free(L);
    free(R);
}

int check(int cmd, int shift) { //shift - смещение
    int mask = 1 << shift;
    return cmd & mask;
}

void execute(int n, int cmd, Deq * L, Deq * R) {
    // Take the need structure
    int Take, flag = 0;
    if (check(cmd, 3) && R->len != 0) {    // Take from R deck
        if (check(cmd, 2)) {               // Take from end
            Take = deDeqend(R, n);
            flag = 1;
        }
        else {                             // Take from top
            Take = deDeqtop(R, n);
            flag = 1;
        }
    }
    if (!check(cmd,3) && L->len != 0) {    // Take from L deck
        if (check(cmd, 2)) {               // Take from end
            Take = deDeqend(L, n);
            flag = 1;
        }
        else {                             // Take from top
            Take = deDeqtop(L, n);
            flag = 1;
        }
    }
    // Put the taken structure
    if (flag) {
        if (check(cmd, 1)) {        // Put (* Take) in R deck
            if (check(cmd, 0)) {    // Put (* Take) down
                enDeqend(R, Take, n);
            } else {                // Put (* Take) up
                enDeqtop(R, Take, n);
            }
        } else {                    // Put (* Take) in L deck
            if (check(cmd, 0)) {    // Put (* Take) down
                enDeqend(L, Take, n);
            } else {                // Put (* Take) up
                enDeqtop(L, Take, n);
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

void cognates(int N, int M, Deq * L, Deq * R) {
    char command;
    for (int i = 0; i < M; i++) {
        scanf("%c", &command);

        execute(N, defineNumber(command), L, R);
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N, M;
    scanf("%d %d\n", &N, &M);

    Deq * deckLeft = createDeq(N, 1);   // from 1 to N, step = 1
    Deq * deckRight = createDeq(N, -1); // from -1 to -N, step = -1

    cognates(N, M, deckLeft, deckRight);   // тасовка

    conclusion(N, deckLeft, deckRight);

    return 0;
}