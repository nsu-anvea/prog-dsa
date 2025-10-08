#include <stdio.h>
#include <stdlib.h>

int * createLinearOrder(int size) {
    int * S = (int *)malloc(sizeof(int) * size);
    return S;
}

int ** createMatrix(int size) {
    int ** S = (int **)malloc(sizeof(int *) * (size + 1));

    for (int i = 0; i < (size + 1); i++) {
        S[i] = (int *)malloc(sizeof(int) * size);

        for (int j = 0; j < size; j++) {
            S[i][j] = 0;
        }
    }
    return S;
}

void toEmpty(int ** S, int size) {
    for (int i = 0; i < size + 1; i++) free(S[i]);
    free(S);
}

void deletingLinks(int ** S, int N, int position) {
    for (int i = 0; i < N; i++) {
        S[0][i] -= S[position][i];
        S[position][i] = 0;
    }
}

int * topologicalSort(int ** S, int N) {
    int * LO = (int *)malloc(sizeof(int) * N);
    int len = 0;

    int flag, k = 0;
    while (k != N) {
        flag = 0;
        for (int j = 0; j < N; j++) {
            if (!S[0][j]) {
                LO[len] = j + 1;
                len++;

                deletingLinks(S, N, j + 1);
                flag = 1;
                k++;

                S[0][j] = -1; // Проверен
                break;
            }
        }
        if (!flag) {
            if (k != N) {
                printf("bad course");
                exit(0);
            }
        }
    }
    return LO;
}

void execute(int N, int M) {
    int ** matrix = createMatrix(N);

    int x, y;
    for (int i = 0; i < M; i++) {
        scanf("%d %d\n", &x, &y);

        if (!matrix[x][y - 1]) { // Проверка на дубликат связи
            matrix[x][y - 1] = 1;
            matrix[0][y - 1]++;
        }
    }
    int * linearOrder = topologicalSort(matrix, N); // Linear order - линейный порядок
    for (int i = 0; i < N; i++) printf("%d ", linearOrder[i]);

    toEmpty(matrix, N);
    free(linearOrder);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N, M;
    scanf("%d %d\n", &N, &M);

    execute(N, M);

    return 0;
}