#include <stdio.h>
#include <stdlib.h>
#define size 105

typedef struct node {
    char * a;
    struct node * left;
    struct node * right;
} tree;

tree * createTree(char * x) {
    tree * S = (tree *)malloc(sizeof(tree));
    S->a = (char *)malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++) {
        S->a[i] = x[i];
    }
    S->left = NULL;
    S->right = NULL;

    return S;
}

void conclusionInf(tree * S) {
    if (!S) return;
    conclusionInf(S->left);
    printf("%s\n", S->a);
    conclusionInf(S->right);
}

int compare(tree * S, char * x) {
    int i = 0;
    while (S->a[i] != '\0' && x[i] != '\0') {
        if (x[i] > S->a[i]) return 1;
        if (x[i] < S->a[i]) return 0;
        i++;
    }
    if (S->a[i] == '\0' && x[i] == '\0') return 2;
    if (x[i] != '\0') return 1;
    return 0;
}

tree * add(tree * t, char * x) {
    if (!t) {
        t = createTree(x);
        return t;
    }
    if (compare(t, x) == 0) t->left = add(t->left, x);
    else if (compare(t, x) == 1) t->right = add(t->right, x);
    return t;
}

void execute(tree * S) {
    char * x = (char *)malloc(sizeof(char) * size);
    while (scanf("%s\n", x) > 0) {
        S = add(S, x);
    }
    conclusionInf(S);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    tree * Tree = NULL;

    execute(Tree);

    return 0;
}