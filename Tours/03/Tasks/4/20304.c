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

int cntPeaks(tree * S, int l, int t_level) {
    if (!S) return 0;
    if (t_level == l) return 1;
    return cntPeaks(S->left, l, t_level + 1) + cntPeaks(S->right, l, t_level + 1);
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
    int level;
    scanf("%d\n", &level);
    int x;
    while (scanf("%d", &x) == 1) {
        S = add(S, x);
    }
    printf("%d", cntPeaks(S, level, 0));
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    tree * Tree = NULL;

    execute(Tree);

    return 0;
}