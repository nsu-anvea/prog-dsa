#include <stdio.h>
#include <stdlib.h>

typedef struct couple {
    int a;
    int i;
} Couple;

typedef struct heap {
    int len;
    struct couple * set;
} Heap;

void swap(int * a, int * b) {
    int c = * a;
    * a = * b;
    * b = c;
}

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return (2 * i + 1);
}

int right(int i) {
    return (2 * i + 2);
}

void siftUp(Heap * H, int i) {
    if (!i) return;
    int p = parent(i);
    if (H->set[i].a > H->set[p].a) {
        swap(&H->set[i].a, &H->set[p].a);
        swap(&H->set[i].i, &H->set[p].i);
        siftUp(H, p);
    }
}

void siftDown(Heap * H, int i) {
    int l = left(i);
    int r = right(i);
    int t;
    if (l < H->len && H->set[l].a > H->set[i].a) { // H[i] < H[l] -> переход к t = l
        t = l;
    } else t = i;
    if (r < H->len && H->set[r].a > H->set[t].a) t = r;
    if (t != i) {
        swap(&H->set[i].a, &H->set[t].a);
        swap(&H->set[i].i, &H->set[t].i);
        siftDown(H, t);
    }
}

void push(Heap * H, int x, int i) {
    H->set[H->len].a = x;
    H->set[H->len].i = i;
    siftUp(H, H->len);
    H->len++;
}

int pop(Heap * H) {
    int deleted = H->set[0].a;

    swap(&H->set[0].a, &H->set[H->len - 1].a);
    swap(&H->set[0].i, &H->set[H->len - 1].i);
    H->len--;
    siftDown(H, 0);
    return deleted;
}

Heap * createHeap(int size) {
    Heap * H = (Heap *)malloc(sizeof(Heap));
    H->len = 0;
    H->set = (Couple *)malloc(sizeof(Couple) * size);
    return H;
}

void commands(int N, int * data, Heap * heap) {
    int M = 2 * N - 1;
    int L = 0, R = 0;

    char cmd;
    for (int i = 0; i < M; i++) {
        scanf("%c", &cmd);
        if (cmd == 'R') {
            R++;
            push(heap, data[R - 1], R - 1);
        } else {
            L++;
            while (heap->set[0].i < L || heap->set[0].i >= R) {
                pop(heap);
            }
        }
        printf("%d\n", heap->set[0].a);
    }
}

void execute(int N) {
    int * data = (int *)malloc(sizeof(int) * N);

    for (int i = 0; i < N; i++) {
        scanf("%d", &data[i]);
    }
    char skip;
    scanf("%c", &skip);

    Heap * heap = createHeap(N);
    commands(N, data, heap);

    free(heap->set);
    free(heap);
    free(data);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N;
    scanf("%d\n", &N);

    execute(N);

    return 0;
}