#include <stdio.h>
#include "stack.h"
#include "checks.h"
#include "calc.h"

int main(int argc, char ** argv) {
    if (argc < 3) {
        printf("\nIncorrect input. "
               "Please specify the input and output files.\n\n");
        return 1;
    }
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    char * expression = createExpression(); // expression - выражение

    execute(expression);

    return 0;
}