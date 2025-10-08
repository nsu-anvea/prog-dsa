#include <stdio.h>
#include <stdlib.h>

typedef struct queue {
    int   len;
    int   first;
    int   last;
    int   size;
    int * set;
} Queue;

int lenght(Queue * S) {
    return S->len;
}

int first(Queue * S) {
    return S->first;
}

int last(Queue * S) {
    return S->last;
}

int size(Queue * S) {
    return S->size;
}

void enqueue(Queue * S, int x) {
    if (!S->len) {
        S->first = (first(S) - 1 + size(S)) % size(S);
        S->set[first(S)] = x;
        S->len++;
    } else {
        S->set[last(S)] = x;
        S->last = (last(S) + 1) % size(S);
        S->len++;
    }
}

int dequeue(Queue * S) {
    int d = S->set[first(S)];
    S->set[first(S)] = 0;
    S->first = (first(S) + 1) % size(S);
    S->len--;
    return d;
}

Queue * createQueue(size_t size) {
    Queue * S = (Queue *)malloc(sizeof(Queue));
    S->len = 0;
    S->first = 0;
    S->last = 0;
    S->size = size;
    S->set = (int *)malloc(sizeof(int) * size);
    return S;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    int sizeBuff, requests;
    scanf("%d %d", &sizeBuff, &requests);
    
    Queue * queue = createQueue(sizeBuff);

    int expResult, count;
    for (int i = 0; i < requests; i++) {
        scanf("%d", &count);
        if (count > 0) {
            if (sizeBuff - lenght(queue) < count)
                printf("Memory overflow\n");
            for (int j = 0; j < count; j++) {
                scanf("%d", &expResult);
                i++;
                if (lenght(queue) < sizeBuff)
                    enqueue(queue, expResult);
            }
        }
        else {
            // counting average (среднее значение)
            int sum = 0, cnt = 0, len = lenght(queue);
            for (int j = 0; j < count * (-1); j++) {
                if (lenght(queue) > 0) {
                    sum += dequeue(queue);
                    cnt++;
                }
            }
            if (cnt != 0)
                printf("%d\n", sum / cnt);

            if (len < count * (-1))
                printf("Empty queue\n");
        }
    }

    /* Clearning the memory */
    free(queue->set);
    free(queue);

    return 0;
}