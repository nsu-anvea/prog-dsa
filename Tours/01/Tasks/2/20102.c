#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
    char a;
    struct list * next;
} Element;
typedef struct stack {
    struct list * top;
} Stack;

int isEmpty(Stack * S) {
    if (!S->top) return 1;
    return 0;
}

char top(Stack * S) {
    return (S->top->a);
}

void push(Stack * S, char e) {
    Element * TOP = (Element *)malloc( sizeof(Element) );
    TOP->a = e;
    TOP->next = S->top;
    S->top = TOP;
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

Stack * create() {
    Stack * S = (Stack *)malloc(sizeof(Stack));
    S->top = NULL;
    return S;
}

int isLetter(char x) {
    if ('a' <= x && x <= 'z') {
        return 1;
    }
    return 0;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    char ipt[1000];
    scanf("%s", ipt);

    char str[1000] = {'\0'};
    int len = 0;
    
    Stack * head = create();

    char x;
    int i = 0;
    while (ipt[i] != '\0') {
        x = ipt[i];
        if (isLetter(x)) {
            str[len] = x;
            len++;
        } 
        else {
            if (x == '(') {
                push(head, x);
            }
            else if (x == ')') {
                while (top(head) != '(') {
                    str[len] = top(head);
                    len++;
                    pop(head);
                }
                pop(head);
            }
            else {
                while (!isEmpty(head) &&
                    (
                        (
                            (top(head) == '*' || top(head) == '/') &&
                            (x == '*' || x == '/' || x == '+' || x == '-')
                        ) || (
                            (top(head) == '+' || top(head) == '-') && 
                            (x == '+' || x == '-')
                        )
                    )
                ) {
                    str[len] = top(head);
                    len++;
                    pop(head);
                }
                push(head, x);
            }
        }
        i++;
    }
    while (!isEmpty(head)) {
        str[len] = top(head);
        len++;
        pop(head);
    }
    free(head);

    //Conclusion
    printf("%s\n", str);

    return 0;
}