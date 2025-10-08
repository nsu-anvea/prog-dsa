#include <stdio.h>
#include <stdlib.h>
#define uint unsigned int

uint ** create_matrix_adj(size_t size) {
    uint ** S = (uint **)calloc(size, sizeof(uint *));
    for (int i = 0; i < size; i++) {
        S[i] = (uint *)calloc(size / 32 + 1, sizeof(uint));

        char connection;
        for (int j = 0; j < size; j++) {
            scanf("%c", &connection);
            if (connection == '1')
                S[i][j / 32] |= (uint)1 << (31 - j % 32);
        }
        scanf("%c", &connection);
    }
    return S;
}

void transitive_closure(uint ** matrix_adj, size_t size) {
    for (int k = 0; k < size; k++)
        for (int i = 0; i < size; i++)
            if (matrix_adj[i][k / 32] & ((uint)1 << (31 - k % 32)))
                for (int j = 0; j < size / 32 + 1; j++)
                    matrix_adj[i][j] |= matrix_adj[k][j];
}

void reflexive_closure(uint ** matrix_adj, size_t size) {
    for (int i = 0; i < size; i++)
        matrix_adj[i][i / 32] |= (uint)1 << (31 - i % 32);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int vertices_count;
    scanf("%d\n", &vertices_count);

    uint ** matrix_adjacencies = create_matrix_adj(vertices_count);

    transitive_closure(matrix_adjacencies, vertices_count);
    reflexive_closure(matrix_adjacencies, vertices_count);

    for (int i = 0; i < vertices_count; i++) {
        for (int j = 0; j < vertices_count; j++) {
            if (matrix_adjacencies[i][j / 32] & (((uint)1 << (31 - j % 32))))
                putchar('1');
            else
                putchar('0');
        }
        putchar('\n');
    }

    /* Clearning the memory */
    for (int i = 0; i < vertices_count; i++)
        free(matrix_adjacencies[i]);
    free(matrix_adjacencies);

    return 0;
}