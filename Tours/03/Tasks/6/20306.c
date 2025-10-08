#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char a;
    struct node * left;
    struct node * right;
} tree;

tree * createTree(char x) {
    tree * S = (tree *)malloc(sizeof(tree));
    S->a = x;
    S->left = NULL;
    S->right = NULL;
    return S;
}

int isNumber(char a) {
    if ('0' <= a && a <= '9') {
        return 1;
    }
    return 0;
}

int isSign(char a) {
    if (a == '*' || a == '/' || a == '+' || a == '-') {
        return 1;
    }
    return 0;
}

int calculate(tree * S) {
    if (isNumber(S->a)) {
        return (S->a - '0');
    }
    if (S->a == '*') return calculate(S->left) * calculate(S->right);
    else if (S->a == '/') {
        if (S->right->a != '0') return calculate(S->left) / calculate(S->right);
        else {
            printf("NO");
            exit(0);
        }
    }
    else if (S->a == '+') return calculate(S->left) + calculate(S->right);
    else return calculate(S->left) - calculate(S->right);
}

tree * add(tree * t, char x) {
    scanf("%c", &x);
    if (!t) t = createTree(x);
    if (isNumber(x)) return t;
    if (isSign(x)) {
        t->left = add(t->left, x);
        t->right = add(t->right, x);
    }
    return t;
}

void execute(tree * S) {
    char x = '\0';
    S = add(S, x);
    printf("%d", calculate(S));
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    tree * Tree = NULL;

    execute(Tree);

    return 0;
}