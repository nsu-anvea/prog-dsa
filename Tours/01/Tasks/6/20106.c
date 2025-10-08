#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
    char a;
    double b;
    struct list * next;
} Element;
typedef struct stack {
    struct list * top;
} Stack;

typedef struct numO {
    int number;
    struct numO * nextN;
    struct Rsign * nextS;
} NumO;

typedef struct Rsign {
    char sign;
    struct numO * nextN;
    struct Rsign * nextS;
} Rsign;

typedef struct numORsign {
    int len;
    struct numO * firstN;
    struct numO * lastN;
    struct Rsign * lastS;
} NumORsign;

void push(Stack * S, char e) {
    Element * TOP = (Element *)malloc( sizeof(Element) );

    TOP->a = e;
    TOP->next = S->top;
    S->top = TOP;
}

void push_num(Stack * S, double e) {
    Element * TOP = (Element *)malloc( sizeof(Element) );

    TOP->b = e;
    TOP->next = S->top;
    S->top = TOP;
}

void add(NumORsign * S, int n, char s, char action) {
    if (action == 's') {
        Rsign * Add = (Rsign *)malloc( sizeof(Rsign) );
        Add->sign = s;
        Add->nextN = NULL;
        Add->nextS = NULL;
        if (S->lastN == NULL) {
            S->lastS->nextS = Add;
        } else {
            S->lastN->nextS = Add;
        }
        S->lastS = Add;
        S->lastN = NULL;

        S->len++;
    } else if (action == 'n') {
        NumO * Add = (NumO *)malloc( sizeof(NumO) );
        Add->number = n;
        Add->nextN = NULL;
        Add->nextS = NULL;
        if (S->firstN == NULL) {
            S->firstN = Add;
            S->lastN = Add;
        } else {
            if (S->lastN == NULL) {
                S->lastS->nextN = Add;
            } else {
                S->lastN->nextN = Add;
            }
            S->lastN = Add;
            S->lastS = NULL;
        }
        S->len++;
    }
}

char pop(Stack * S) {
    Element * will_poped;
    will_poped = S->top;

    S->top = will_poped->next;

    char e;
    e = will_poped->a;

    free(will_poped);
    return e;
}

char pop_num(Stack * S) {
    Element * will_poped;
    will_poped = S->top;

    S->top = will_poped->next;

    double e;
    e = will_poped->b;

    free(will_poped);
    return e;
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
        r = (a / b);
    }

    return r;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    char * str = (char *)malloc( sizeof(char) * 500010 );
    fgets(str, 500010, stdin);

    int len_s = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            str[len_s] = str[i];
            len_s++;
        }
    }
    if (str[len_s - 1] == '\n') {
        str[len_s - 1] = '\0';
        len_s--;
    }

    Stack * stack = (Stack *)malloc( sizeof(Stack) );
    stack->top = NULL;

    NumORsign * Post_note = (NumORsign *)malloc( sizeof(NumORsign) );
    Post_note->len = 0;
    Post_note->firstN = NULL;
    Post_note->lastN = NULL;
    Post_note->lastS = NULL;

//________________________________Creating a Post_note
    int num;
    char sign;
    int i = 0, flag = 0;
    while (i < len_s) {
        if ('0' <= str[i] && str[i] <= '9') {
            num = 0;
            while ('0' <= str[i] && str[i] <= '9') {
                num = num * 10 + (int)(str[i] - '0');
                i++;
            }
            i--;
            add(Post_note, num, sign, 'n');
        }
        else if (
            (flag == 0) && (
                (str[i] == '-' && i == 0) ||
                (str[i] == '-' && i != 0 && str[i - 1] == '(')
                )
        ) {
            add(Post_note, 0, sign, 'n');
            flag = 1;
            i--;
        }
        else if (str[i] != ' ') {
            sign = str[i];
            if (sign == '(') {
                push(stack, sign);
            }
            else if (sign == ')') {
                while (stack->top->a != '(') {
                    add(Post_note, num, stack->top->a, 's');
                    pop(stack);
                }
                pop(stack);
            }
            else {
                while ((stack->top != NULL) &&
                    (
                        (
                            (stack->top->a == '*' || stack->top->a == '/') &&
                            (sign == '*' || sign == '/' || sign == '+' || sign == '-')
                        ) || (
                            (stack->top->a == '+' || stack->top->a == '-') &&
                            (sign == '+' || sign == '-')
                        )
                    )
                ) {
                    add(Post_note, num, stack->top->a, 's');
                    pop(stack);
                }
                flag = 0;
                push(stack, sign);
            }
        }
        i++;
    }
    while (stack->top != NULL) {
        add(Post_note, num, stack->top->a, 's');
        pop(stack);
    }
//____________________________________________________//

//________________________________Counting the Post_note
    NumO * nowN = Post_note->firstN;
    Rsign * nowS;
    double res;
    char position = 'n';
    for (int j = 0; j < Post_note->len; j++) {
        if (position == 'n') {
            push_num(stack, nowN->number);
        }
        else if (stack->top != NULL && stack->top->next != NULL && nowS->sign) {
            res = count(stack->top->b, stack->top->next->b, nowS->sign);
            pop_num(stack);
            pop_num(stack);
            push_num(stack, res);
        }

        //next
        if (position == 'n') {
            if (nowN->nextN == NULL && nowN->nextS == NULL) {
                break;
            }
            if (nowN->nextS == NULL) { // next is a number
                nowN = nowN->nextN;
                position = 'n';
            } else { // next is a sign
                nowS = nowN->nextS;
                position = 's';
            }
        }
        else {
            if (nowS->nextN == NULL && nowS->nextS == NULL) {
                break;
            }
            if (nowS->nextN == NULL) { // next is a sign
                nowS = nowS->nextS;
                position = 's';
            } else { // next is a number
                nowN = nowS->nextN;
                position = 'n';
            }
        }
    }

    printf("%.20lf", stack->top->b);

    free(stack->top);
    free(stack);

    return 0;
}