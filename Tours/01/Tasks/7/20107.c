#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 1000010

// Index designations:
// 0 - (
// 1 - {
// 2 - [
// 3 - <
typedef struct el {
    int array[4];
} El;

typedef struct stack {
    int top;
    int len;
    struct el * set;
} Stack;

int top(Stack * S) {
    return (S->top);
}

void addParentheses(int array[4], char x) {
    if          (x == '(' || x == ')') array[0]++;
    else if     (x == '{' || x == '}') array[1]++;
    else if     (x == '[' || x == ']') array[2]++;
    else if     (x == '<' || x == '>') array[3]++;
}

void push(Stack * S, char x) {
    S->top = (top(S) - 1 + size) % size;
    S->len++;
    addParentheses(S->set[top(S)].array, x); // Parentheses - скобки
}

void pop(Stack * S) {
    S->top = (top(S) + 1) % size;
    S->len--;
}

Stack * createStack() {
    Stack * S = (Stack *)malloc(sizeof(Stack));
    S->top = 0;
    S->len = 0;
    S->set = (El *)malloc(sizeof(El) * size);

    for (int i = 0; i < size; i++) {
        S->set[i].array[0] = 0;
        S->set[i].array[1] = 0;
        S->set[i].array[2] = 0;
        S->set[i].array[3] = 0;
    }
    return S;
}

void emptyStack(Stack * S) {
    free(S->set);
    free(S);
}

void clearStack(Stack * S) {
    for (int i = 0; i < S->len; i++) {
        S->set[top(S) + i].array[0] = 0;
        S->set[top(S) + i].array[1] = 0;
        S->set[top(S) + i].array[2] = 0;
        S->set[top(S) + i].array[3] = 0;
    }
    S->top = 0;
    S->len = 0;
}

El * createARP() { // create an array of back parenthese
    El * S = (El *)malloc(sizeof(El));
    S->array[0] = 0;
    S->array[1] = 0;
    S->array[2] = 0;
    S->array[3] = 0;
    return S;
}

void clearARP(El * S) {
    S->array[0] = 0;
    S->array[1] = 0;
    S->array[2] = 0;
    S->array[3] = 0;
}

char * createSequence() {
    char * S = (char *)malloc(sizeof(char) * size);
    return S;
}

int sumEl(int arr[4]) {
    if (arr[0] || arr[1] || arr[2] || arr[3]) return 1;
    return 0;
}

int isOpen(int x) {
    if (
            x == '(' ||
            x == '{' ||
            x == '[' ||
            x == '<'
            ) return 1;
    return 0;
}

int isClose(int x) {
    if (
            x == ')' ||
            x == '}' ||
            x == ']' ||
            x == '>'
            ) return 1;
    return 0;
}

void subtract(int a[4], int b[4]) { // Вычитать
    for (int i = 0; i < 4; i++) {
        if (a[i]) {
            if (a[i] >= b[i]) {
                a[i] -= b[i];
                b[i] = 0;
            } else {
                b[i] -= a[i];
                a[i] = 0;
            }
        }
    }
}

int checkCntParenthese(char * seq) {
    int cntParenthese[4] = {0, 0, 0, 0};
    for (int i = 0; seq[i] != '\n' && seq[i] != '\0'; i++) {
        if (seq[i] == '(') cntParenthese[0]++;
        if (seq[i] == '{') cntParenthese[1]++;
        if (seq[i] == '[') cntParenthese[2]++;
        if (seq[i] == '<') cntParenthese[3]++;
        if (seq[i] == ')') cntParenthese[0]--;
        if (seq[i] == '}') cntParenthese[1]--;
        if (seq[i] == ']') cntParenthese[2]--;
        if (seq[i] == '>') cntParenthese[3]--;
    }
    if (cntParenthese[0] == 0 &&
        cntParenthese[1] == 0 &&
        cntParenthese[2] == 0 &&
        cntParenthese[3] == 0) return 1;
    return 0;
}

int checkIt(char * seq, El * arp, Stack * S) {
    fgets(seq, size, stdin);

    if (!checkCntParenthese(seq)) return 0;

    int i = 0;
    while (seq[i] != '\n' && seq[i] != '\0') {
        if (isOpen(seq[i])) {
            push(S, seq[i]);
            i++;
            while (isOpen(seq[i])) {
                addParentheses(S->set[top(S)].array, seq[i]);
                i++;
            }
        }
        while (isClose(seq[i])) {
            addParentheses(arp->array, seq[i]);
            i++;
        }
        subtract(S->set[top(S)].array, arp->array);

        if (sumEl(S->set[top(S)].array) && (sumEl(arp->array))) return 0;
        if (!sumEl(S->set[top(S)].array) && (sumEl(arp->array))) {
            for (int j = 1; j < S->len && sumEl(arp->array); j++) {
                subtract(S->set[(top(S) + j) % size].array, arp->array);
                if (sumEl(S->set[top(S)].array) && (sumEl(arp->array))) return 0;
            }
            if (sumEl(arp->array)) {
                return 0;
            }
        }
    }
    if (!sumEl(arp->array)) {
        i = 0;
        while (i < S->len) {
            if (sumEl(S->set[top(S) + i].array)) return 0;
            i++;
        }
    }
    return 1;
}

void execute(int n) {
    Stack * stack = createStack();
    El * ARP = createARP(); // array of back parenthese

    char * sequence = createSequence();
    for (int i = 0; i < n; i++) {
        if (checkIt(sequence, ARP, stack)) {
            printf("T\n");
        } else {
            printf("NIL\n");
        }
        clearStack(stack);
        clearARP(ARP);
    }
    emptyStack(stack);
    free(ARP);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N;
    scanf("%d\n", &N);

    execute(N);

    return 0;
}