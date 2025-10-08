#include <stdio.h>
#include <stdlib.h>
#define sizeEn 26

typedef struct node {
    char a;
    struct node * left;
    struct node * right;
} tree;

tree * createTree(char x) {
    tree * T = (tree *)malloc(sizeof(tree));
    T->a = x;
    T->left = NULL;
    T->right = NULL;
    return T;
}

void conclusionStraight(tree * T) {
    if (!T) return;
    printf("%c", T->a);
    conclusionStraight(T->left);
    conclusionStraight(T->right);
}

tree * add(tree * T, char x) {
    if (!T) {
        T = createTree(x);
        return T;
    }
    if (x < T->a) T->left = add(T->left, x);
    else if (x > T->a) T->right = add(T->right, x);
    return T;
}

void execute(tree * T) {
    char EnglishLetters[sizeEn];
    int position = 1;
    char x;
    while (scanf("%c", &x) == 1) {
        if (x != '\n') {
            EnglishLetters[sizeEn - position] = x;
            position++;
        }
    }
    position = sizeEn - position + 1;
    while (position < sizeEn) {
        T = add(T, EnglishLetters[position]);
        position++;
    }
    conclusionStraight(T);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    tree * Tree = NULL;

    execute(Tree);

    return 0;
}