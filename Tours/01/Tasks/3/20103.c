#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
    int a;
    struct list * next;
} Element;

typedef struct stack {
    struct list * top;
} Stack;

int top(Stack * S) {
    return (S->top->a);
}

void push(Stack * S, int e) {
    Element * TOP = (Element *)malloc( sizeof(Element) );
    TOP->a = e;
    TOP->next = S->top;
    S->top = TOP;
}

int pop(Stack * S) {
    Element * will_poped;
    will_poped = S->top;

    S->top = will_poped->next;

    int e;
    e = will_poped->a;

    free(will_poped);
    return e;
}

Stack * create() {
    Stack * S = (Stack *)malloc(sizeof(Stack));
    S->top = NULL;
    return S;
}

int isDigit(char x) {
    if ('0' <= x && x <= '9') {
        return 1;
    }
    return 0;
}

int count(int b, int a, char action) {
    int res;

    if (action == '+') {
        res = a + b;
    }
    else if (action == '-') {
        res = a - b;
    }
    else if (action == '*') {
        res = a * b;
    }
    else {
        res = a / b;
    }

    return res;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    char ipt[2001];
    fgets(ipt, sizeof(ipt), stdin);

    int ipt_len = strlen(ipt);

    ipt[ipt_len - 1] = '\0';

    Stack * stack = create();

    int x, prom;
    char y;
    int i = 0;
    while (ipt[i] != '\0') {
        y = ipt[i];

        if (isDigit(y)) {
            x = 0;
            while (ipt[i] != ' ' && ipt[i] != '\0') {
                x = x * 10 + (int)(ipt[i] - 48);
                i++;
            }
            push(stack, x);

            if (ipt[i] == '\0') {
                break;
            }
        }
        else {
            int a = pop(stack);
            int b = pop(stack);
            prom = count(a, b, y);
            push(stack, prom);
            i++;
        }
        i++;
    }

    printf("%d", top(stack));
    free(stack);

    return 0;
}