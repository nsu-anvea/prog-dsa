#include <stdio.h>
#include <stdlib.h>
#define size 500500

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
    if (H->set[i] > H->set[p]) {
        swap(&H->set[i], &H->set[p]);
        siftUp(H, p);
    }
}

void siftDown(Heap * H, int i) {
    int l = left(i);
    int r = right(i);
    int t;
    if (l < H->len && H->set[l] > H->set[i]) { // H[i] < H[l] -> переход к t = l
        t = l;
    } else t = i;
    if (r < H->len && H->set[r] > H->set[t]) t = r;
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

void conclusion(Heap * H) {
    for (int i = 0; i < H->len; i++) {
        fwrite(&H->set[i], sizeof(int), 1, stdout);
    }
}

void sortOnTheHeap(Heap * H) {
    int len = H->len;
    for (int i = len - 1; i > 0; i--) {
        swap(&H->set[i], &H->set[0]);
        H->len--;
        siftDown(H, 0);
    }
    H->len = len;
}

void buildingHeap(Heap * H, int start) {
    for (int i = start; i >= 0; i--) {
        siftDown(H, i);
    }
}

void heapSort(Heap * H) {
    // Building heap
    int middle = (H->len - 1) / 2;
    buildingHeap(H, middle);

    // Sorting on the heap
    sortOnTheHeap(H);
}

void execute(int n) {
    Heap * heap = createHeap();

    int x;
    for (int i = 0; i < n; i++) {
        fread(&x, sizeof(int), 1, stdin);
        heap->set[i] = x;
        heap->len++;
    }
    heapSort(heap);
    conclusion(heap);
    free(heap->set);
    free(heap);
}

int main() {
    freopen("input.bin", "rb", stdin);
    freopen("output.bin", "wb", stdout);

    int N;
    fread(&N, sizeof(int), 1, stdin);

    execute(N);

    return 0;
}