#include <stdio.h>
#include <stdlib.h>

#include "calc.h"
#define size 500010

char * createExpression() {
    char * S = (char *)malloc(sizeof(char) * size);
    fgets(S, size, stdin);

    int cntStaples = 0;

    int len = 0;
    for (int i = 0; S[i] != '\0'; i++) {
        if (S[i] != ' ') {
            if (isSign(S[i]) && i) {
                if (isSign(S[i - 1])) {
                    printf("\nTwo signs side by side @o@\n\n");
                    exit(1);
                }
                if (S[i] == '/' && S[i + 1] == 0) {
                    printf("\nYou can't divide by zero. Learn math)\n\n");
                    exit(1);
                }
            }
            if (S[i] == '(' || S[i] == ')' || S[i] == '\n' || isDigit(S[i]) || isSign(S[i])) {
                S[len] = S[i];
                len++;
                if (S[i] == '(') cntStaples++;
                else if (S[i] == ')') cntStaples--;
            } else {
                printf("\nBim bam boom :D This calculator can only count numbers\n\n");
                exit(1);
            }
        }
    }
    if (cntStaples) {
        printf("\nMaybe you've lost a bracket?\n\n");
        exit(1);
    }
    S[len] = '\0';
    if (S[len - 1] == '\n') {
        S[len - 1] = '\0';
        len--;
    }
    return S;
}

void append(char * postEntry, int * len, char x) {
    postEntry[*len] = x;
    (*len)++;
    postEntry[*len] = ' ';
    (*len)++;
}

char * createPostEntry(char * ex, Stack * S, int * lenPostEntry) {
    char * postEntry = (char *)malloc(sizeof(char) * size);

    int i = 0, flag = 0;
    while (ex[i] != '\0') {
        if (isDigit(ex[i])) {
            while (isDigit(ex[i])) {
                postEntry[*lenPostEntry] = ex[i];
                (*lenPostEntry)++;
                i++;
            }
            postEntry[*lenPostEntry] = ' ';
            (*lenPostEntry)++;
            i--;
        }
        else if (flag == 0 && isUnaryMinus(ex, i)) {
            append(postEntry, lenPostEntry, '0');
            flag = 1;
            i--;
        }
        else {
            if (ex[i] == '(') {
                push(S, ex[i]);
            }
            else if (ex[i] == ')') {
                while (top(S) != '(') {
                    append(postEntry, lenPostEntry, (char)top(S));
                    pop(S);
                }
                pop(S);
            }
            else {
                while (S->top && isRightPriority(S, ex[i])) {
                    append(postEntry, lenPostEntry, (char)top(S));
                    pop(S);
                }
                flag = 0;
                push(S, ex[i]);
            }
        }
        i++;
    }
    while (S->top) {
        append(postEntry, lenPostEntry, (char)top(S));
        pop(S);
    }
    return postEntry;
}

double count(double b, double a, char action) {
    double r;

    if (action == '+') {
        r = a + b;
    }
    else if (action == '-') {
        r = a - b;
    }
    else if (action == '*') {
        r = a * b;
    }
    else {
        if (b) {
            r = (a / b);
        } else {
            printf("\nYou can't divide by zero. Learn math)\n\n");
            exit(1);
        }
    }

    return r;
}

double calculate(char * postEntry, int len, Stack * S) {
    double res;

    int i = 0;
    double number;
    while (i < len) {
        if (isDigit(postEntry[i])) {
            number = 0;
            while (postEntry[i] != ' ') {
                number = number * 10 + (double)(postEntry[i] - '0');
                i++;
            }
            push(S, number);
        }
        else if (isSign(postEntry[i])) {
            res = count(S->top->a, S->top->next->a, postEntry[i]);
            pop(S);
            pop(S);
            push(S, res);
        }
        i++;
    }
    return res;
}

void execute(char * ex) {
    Stack * stack = createStack();
    int lenPostEntry = 0;
    char * postEntry = createPostEntry(ex, stack, &lenPostEntry);
    clear(stack);
    calculate(postEntry, lenPostEntry, stack);
    printf("\nHere is Your answer: %.12lf\n\n", (double)top(stack));
    clear(stack);
    free(stack);
}