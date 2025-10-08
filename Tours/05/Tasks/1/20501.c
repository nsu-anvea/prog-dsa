#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 300000

typedef struct heap {
    int len;
    int * set;
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
    if (H->set[i] < H->set[p]) {
        swap(&H->set[i], &H->set[p]);
        siftUp(H, p);
    }
}

void siftDown(Heap * H, int i) {
    int l = left(i);
    int r = right(i);
    int t;
    if (l < H->len && H->set[l] < H->set[i]) { // H[i] < H[l] -> переход к t = l
        t = l;
    } else t = i;
    if (r < H->len && H->set[r] < H->set[t]) t = r;
    if (t != i) {
        swap(&H->set[i], &H->set[t]);
        siftDown(H, t);
    }
}

void push(Heap * H, int x) {
    H->set[H->len] = x;
    siftUp(H, H->len);
    H->len++;
}

int pop(Heap * H) {
    if (!H->len) return size;

    int deleted = H->set[0];
    
    swap(&H->set[0], &H->set[H->len - 1]);
    H->len--;
    siftDown(H, 0);
    return deleted;
}

Heap * createHeap() {
    Heap * H = (Heap *)malloc(sizeof(Heap));
    H->len = 0;
    H->set = (int *)malloc(sizeof(int) * size);
    return H;
}

void run(Heap * H, char act[10]) {
    if (act[0] == 'p' && act[1] == 'u') {       // if push
        int x;
        scanf("%d\n", &x);
        push(H, x);
    }
    else if (act[0] == 'p' && act[1] == 'o') {  // if pop
        int deleted = pop(H);
        if (deleted == size) {
            printf("Heap is empty\n");
        } else {
            printf("%d\n", deleted);
        }
    }
}

void execute(int n) {
    Heap * heap = createHeap();

    char action[10];
    for (int i = 0; i < n; i++) {
        scanf("%s", action);
        run(heap, action);
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N;
    scanf("%d\n", &N);

    execute(N);

    return 0;
}