#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int a;
    int cnt;
    struct node * left;
    struct node * right;
} Tree;

Tree * createNode(int x) {
    Tree * N = (Tree *)malloc(sizeof(Tree));
    N->a = x;
    N->cnt = 1;
    N->left = NULL;
    N->right = NULL;
    return N;
}

Tree * add(Tree * T, int x) {
    if (!T) {
        T = createNode(x);
        return T;
    }
    if          (x < T->a) T->left = add(T->left, x);
    else if     (x > T->a) T->right = add(T->right, x);
    else        (T->cnt)++;
    return T;
}

void swap(int * a, int * b) {
    int c = * a;
    * a = * b;
    * b = c;
}

void conclusion(Tree * T) {
    if (!T) return;
    conclusion(T->left);
    for (int i = 0; i < T->cnt; i++) {
        fwrite(&T->a, sizeof(int), 1, stdout);
    }
    conclusion(T->right);
}

int * createSequence(int n) {
    int * S = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        fread(&S[i], sizeof(int), 1, stdin);
    }
    return S;
}

void shuffle(int * S, int n) {
    for (int i = 0; i < n; i++) {
        swap(&S[i], &S[rand() % n]);
    }
}

void execute(int n) {
    Tree * T = NULL;

    int * sequence = createSequence(n);
    shuffle(sequence, n);

    int x;
    for (int i = 0; i < n; i++) {
        x = sequence[i];
        T = add(T, x);
    }
    conclusion(T);
}

int main() {
    freopen("input.bin", "rb", stdin);
    freopen("output.bin", "wb", stdout);

    int N;
    fread(&N, sizeof(int), 1, stdin);

    execute(N);

    return 0;
}