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
    for (int i = 0; i < size; i++) free(S[i]);
    free(S);
}

void execute(int N, int M) {
    int * linearOrder = createLinearOrder(N);
    int len = 0;
    int ** matrix = createMatrix(N);

    int x, y;
    for (int i = 0; i < M; i++) {
        scanf("%d %d\n", &x, &y);

        matrix[0][y - 1] -= matrix[x][y - 1] + 1;
        matrix[x][y - 1] -= matrix[x][y - 1] + 1;
    }
    int flag, k = 0;
    while (k != N) {
        flag = 0;
        for (int j = 0; j < N; j++) {
            if (!matrix[0][j]) {
                flag = 1;
                k++;
                for (int i = 0; i < N; i++) {
                    matrix[0][i] -= matrix[j + 1][i];
                    matrix[j + 1][i] = 0;
                }
                matrix[0][j] = -1;
                linearOrder[len] = j + 1;
                len++;
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
    for (int l = 0; l < len; l++) printf("%d ", linearOrder[l]);
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