#include <stdio.h>
#include <stdlib.h>

#include "checks.h"

int isDigit(char x) {
    if ('0' <= x && x <= '9') return 1;
    return 0;
}

int isSign(char x) {
    if (
        x == '+' || x == '-' ||
        x == '*' || x == '/'
    ) return 1;
    return 0;
}

int isUnaryMinus(char * ex, int i) {
    if (
        (ex[i] == '-' && i == 0) ||
        (ex[i] == '-' && i != 0 && ex[i - 1] == '(')
    ) return 1;
    return 0;
}

int isRightPriority(Stack * S, char x) {
    if (
        (
            (top(S) == '*' || top(S) == '/') &&
            (x == '*' || x == '/' || x == '+' || x == '-')
        ) || (
            (top(S) == '+' || top(S) == '-') &&
            (x == '+' || x == '-')
        )
    ) return 1;
    return 0;
}