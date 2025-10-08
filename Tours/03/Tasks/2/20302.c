#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int a;
    struct node * left;
    struct node * right;
} tree;

tree * createTree(int x) {
    tree * S = (tree *)malloc(sizeof(tree));
    S->a = x;
    S->left = NULL;
    S->right = NULL;

    return S;
}

int Max(int a, int b) {
    if (a >= b) return a;
    else return b;
}

int maxDepth(tree * S, int depth) {
    if (!S) {
        return depth - 1;
    }
    depth = Max(
            maxDepth(S->left, depth + 1),
            maxDepth(S->right, depth + 1)
            );
    return depth;
}

tree * add(tree * t, int x) {
    if (!t) {
        t = createTree(x);
        return t;
    }
    if (x < t->a) t->left = add(t->left, x);
    else if (x > t->a) t->right = add(t->right, x);
    return t;
}

void execute(tree * S) {
    int x;
    while (scanf("%d", &x) == 1) {
        S = add(S, x);
    }
    printf("%d", maxDepth(S, 0));
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    tree * Tree = NULL;

    execute(Tree);

    return 0;
}