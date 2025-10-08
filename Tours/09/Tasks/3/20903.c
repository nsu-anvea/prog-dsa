#include <stdio.h>
#define SIZE 2000

int vertex_count;

void feel_matrix_adj(int matrix_adj[SIZE][SIZE]) {
    char x;
    for (int i = 0; i < vertex_count; i++) {
        for (int j = 0; j < vertex_count; j++) {
            scanf("%c", &x);
            if (i == j)
                matrix_adj[i][j] = 1;
            else
                matrix_adj[i][j] = (int)x - '0';
        }
        scanf("%c", &x);
    }
}

void transitive_closure_FW(int matrix_adj[SIZE][SIZE]) {
    for (int k = 0; k < vertex_count; k++) {
        for (int i = 0; i < vertex_count; i++) {
            for (int j = 0; j < vertex_count; j++) {
                if (matrix_adj[i][k] && matrix_adj[k][j])
                    matrix_adj[i][j] = 1;
            }
        }
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d\n", &vertex_count);

    int matrix_adjacencies[SIZE][SIZE] = {0};

    feel_matrix_adj(matrix_adjacencies);

    transitive_closure_FW(matrix_adjacencies);

    for (int i = 0; i < vertex_count; i++) {
        for (int j = 0; j < vertex_count; j++)
            printf("%d", matrix_adjacencies[i][j]);
        printf("\n");
    }

    return 0;
}