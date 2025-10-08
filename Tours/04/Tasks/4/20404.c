#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2000000000

typedef struct node {
    int           value;
    int           height;
    struct node * left;
    struct node * right;
} Node;

Node * createNode(int x) {
    Node * T = (Node *)malloc(sizeof(Node));
    T->value = x;
    T->height = 0;
    T->left = NULL;
    T->right = NULL;
    return T;
}

void swap(Node * a, Node * b) {
    int c = a->value;
    a->value = b->value;
    b->value = c;
}

int maxi(int a, int b) {
    if (a > b) return a;
    else return b;
}

int mini(int a, int b) {
    if (a < b) return a;
    else return b;
}

Node * getMax(Node * T) {
    if (!T->right) return T;
    return getMax(T->right);
}

int getHeight(Node * T) {
    if (!T) return -1;
    else return T->height;
}

void updateHeight(Node * T) {
    T->height = 1 + maxi(getHeight(T->left), getHeight(T->right));
}

int getBalance(Node * T) {
    if (!T) return 0;
    else return getHeight(T->right) - getHeight(T->left);
}

void leftRotate(Node * T) {
    swap(T, T->right);

    Node * temp = T->left;
    T->left = T->right;
    T->right = T->left->right;
    //T->right->left = T->right->right;
    T->left->right = T->left->left;
    T->left->left = temp;

    updateHeight(T->left); // len left has changed
    updateHeight(T); // len node also has change
}

void rightRotate(Node * T) {
    swap(T, T->left);

    Node * temp = T->right;
    T->right = T->left;
    T->left = T->right->left;
    T->right->left = T->right->right;
    T->right->right = temp;

    updateHeight(T->right); // len right has changed
    updateHeight(T); // len node also has change
}

void balance(Node * T) {
    int balance = getBalance(T);
    if (balance == -2) {
        if (getBalance(T->left) == 1) leftRotate(T->left);
        rightRotate(T);
    }
    else if (balance == 2) {
        if (getBalance(T->right) == -1) rightRotate(T->right);
        leftRotate(T);
    }
}

Node * add(Node * T, int x, int * isDupe) {
    if (!T) {
        T = createNode(x);
        updateHeight(T);
        balance(T);
        return T;
    }
    if (x < T->value) T->left = add(T->left, x, isDupe);
    else if (x > T->value) T->right = add(T->right, x, isDupe);
    else {
        * isDupe = 1;
    }

    updateHeight(T);
    balance(T);

    return T;
}

Node * delete(Node * T, int x, int * isRemoved) {
    if (!T) {
        * isRemoved = 0;
        return NULL;
    }
    else if (x < T->value) T->left = delete(T->left, x, isRemoved);
    else if (x > T->value) T->right = delete(T->right, x, isRemoved);
    else {
        if (!T->left && !T->right) {
            free(T);
            return NULL;
        }
        else if (!T->left || !T->right) {
            if (!T->left) T = T->right;
            else T = T->left;
        }
        else {
            Node * maxInLeft = getMax(T->left);
            T->value = maxInLeft->value;
            T->left = delete(T->left, maxInLeft->value, isRemoved);
        }
    }
    if (T) {
        updateHeight(T);
        balance(T);
    }
    return T;
}

int lower(Node * T, int x, int * low) {
    Node * t = T;
    * low = MAX;
    while (t) {
        if (x > t->value) t = t->right;
        else if (x < t->value) {
            * low = mini(* low, t->value);
            t = t->left;
        }
        else {
            * low = t->value;
            return 1;
        }
    }
    if (* low == MAX) return 0;
    return 1;
}

void execute(Node * T, int N) {
    int isDupe;
    int isRemoved;
    int low;

    char action[10];
    int x;
    for (int i = 0; i < N; i++) {
        scanf("%s %d\n", action, &x);

        if (action[0] == 'a') {
            isDupe = 0;
            T = add(T, x, &isDupe);
            if (isDupe) printf("dupe\n");
            else printf("added\n");
        }
        else if (action[0] == 'r') {
            isRemoved = 1;
            T = delete(T, x, &isRemoved);
            if (isRemoved) printf("removed\n");
            else printf("miss\n");
        }
        else {
            if (lower(T, x, &low)) {
                printf("%d\n", low);
            } else printf("###\n");
        }
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N;
    scanf("%d\n", &N);

    Node * Tree = NULL;

    execute(Tree, N);

    return 0;
}