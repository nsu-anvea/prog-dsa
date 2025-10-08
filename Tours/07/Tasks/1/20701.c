#include <stdio.h>
#include <stdlib.h>

int N;

typedef struct coned {
    int a;
    struct coned * next;
} Connected;

typedef struct list {
    int check;
    int populated; // населенный
    struct coned * first;
} List;

List * create() {
    List * S = (List *)malloc(sizeof(List) * N);
    for (int i = 0; i < N; i++) {
        S[i].check = 0;
        S[i].populated = 0;
        S[i].first = NULL;
    }
    return S;
}

void dfs(List * S, int x, int * cnt) {
    S[x].check = 1;
    if (S[x].populated) {
        (* cnt)++;
    }

    Connected * t = S[x].first;
    while (t) {
        if (!S[t->a].check) dfs(S, t->a, cnt);
        t = t->next;
    }
}

void add(List * S, int x, int y) {
    Connected * t = S[x - 1].first;
    Connected * p = NULL;
    int isWrited = 0;
    while (t) {
        if (t->a == y - 1) {
            isWrited = 1;
            break;
        }
        p = t;
        t = t->next;
    }
    if (!isWrited) {
        Connected * A = (Connected *) malloc(sizeof(Connected));
        A->a = y - 1;
        A->next = NULL;
        if (!p) S[x - 1].first = A;
        else p->next = A;
    }
}

void execute() {
    int M;
    scanf("%d\n", &M);

    List * adjacencies = create();

    int x, y;
    for (int i = 0; i < M; i++) {
        scanf("%d %d\n", &x, &y);

        add(adjacencies, x, y);
        add(adjacencies, y, x);
    }

    scanf("%d", &M);

    for (int i = 0; i < M; i++) {
        scanf("%d", &x);
        adjacencies[x - 1].populated = 1;
    }
    x = 0;
    for (int i = 0; i < M; i++) if (adjacencies[i].populated) x = i;

    y = 0;
    dfs(adjacencies, x, &y);

    if (y == M) printf("YES");
    else printf("NO");
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d ", &N);

    execute();

    return 0;
}