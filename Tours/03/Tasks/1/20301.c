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

void conclusionReverse(tree * S) {
    if (!S) return;
    conclusionReverse(S->left);
    conclusionReverse(S->right);
    printf("%d ", S->a);
}

void conclusionStraight(tree * S) {
    if (!S) return;
    printf("%d ", S->a);
    conclusionStraight(S->left);
    conclusionStraight(S->right);
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
    conclusionStraight(S);
    printf("\n");
    conclusionReverse(S);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt",  "w", stdout);

    tree * Tree = NULL;

    execute(Tree);

    return 0;
}