#include <stdlib.h>
#include <string.h>

#include "stack.h"

double top(Stack * S) {
    return S->top->a;
}

void push(Stack * S, double x) {
    Element * A = (Element *)malloc(sizeof(Element));
    A->a = x;
    A->next = S->top;
    S->top = A;
}

double pop(Stack * S) {
    Element * A;
    A = S->top;

    S->top = A->next;

    double deleted;
    deleted = A->a;

    free(A);
    return deleted;
}

Stack * createStack() {
    Stack * S = (Stack *)malloc(sizeof(Stack));
    S->top = NULL;
    return S;
}

void clear(Stack * S) {
    while (S->top) {
        pop(S);
    }
}