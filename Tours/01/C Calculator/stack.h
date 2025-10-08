#pragma once

typedef struct list {
    double a;
    struct list * next;
} Element;

typedef struct stack {
    struct list * top;
} Stack;

double top(Stack * S);
void push(Stack * S, double x);
double pop(Stack * S);
Stack * createStack();
void clear(Stack * S);