#include <stdio.h>
#include <stdlib.h>

int N;
int cntCC; // the count of the connectivity component

typedef struct coned {
    int a;
    struct coned * next;
} Connected;

typedef struct list {
    struct coned * first;
} List;

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

List * create() {
    List * S = (List *)malloc(sizeof(List) * N);
    for (int i = 0; i < N; i++) {
        S[i].first = NULL;
    }
    return S;
}

char * getmemChar() {
    char * S = (char *)malloc(sizeof(char) * N);
    for (int i = 0; i < N; i++) {
        S[i] = '0';
    }
    return S;
}

int * getmemInt() {
    int * S = (int *)malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++) {
        S[i] = 0;
    }
    return S;
}

void dfs(List * S, char * visited, int * arrNumCC, int * len, int x) {
    visited[x] = '1';
    (* len)++;
    arrNumCC[x] = cntCC;
    Connected * t = S[x].first;
    while (t) {
        if (visited[t->a] == '0') dfs(S, visited, arrNumCC, len, t->a);
        t = t->next;
    }
}

void execute() {
    List * adjacencies = create(); // List of adjacencies
    char * visited = getmemChar();
    int lenVisited = 0;
    int * arrayNumCC = getmemInt();

    int x, y;
    while (scanf("%d %d\n", &x, &y) != -1) {
        add(adjacencies, x, y);
        add(adjacencies, y, x);
    }

    cntCC = 0;
    int start;
    while (lenVisited != N) {
        cntCC++;
        for (int i = 0; i < N; i++) {
            if (visited[i] == '0') {
                start = i;
                break;
            }
        }
        dfs(adjacencies, visited, arrayNumCC, &lenVisited, start);
    }
    printf("%d\n", cntCC + 1);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d\n", &N);

    execute();

    return 0;
}