#include <stdio.h>
#include <stdlib.h>
#define INF 1000000000

int point_count;
int ways_count;
int request_path_count;
int request_time_count;

typedef struct path {
    int len;
    int * set;
} Path;

int ** create_table(int lines, int columns) {
    int ** table = (int **)malloc(sizeof(int *) * lines);
    for (int i = 0; i < lines; i++) {
        table[i] = (int *)malloc(sizeof(int) * columns);
        for (int j = 0; j < columns; j++) {
            if (i == j)
                table[i][j] = 0;
            else
                table[i][j] = INF;
        }
    }
    return table;
}

int * get_mem_int(int size) {
    int * S = (int *)malloc(sizeof(int) * size);
    return S;
}

Path * create_path(int size) {
    Path * S = (Path *)malloc(sizeof(Path));
    S->set = get_mem_int(size);
    S->len = 0;
    return S;
}

void add_to_matrix_adj(int ** matrix_adj, int ** previous, int x, int y, int z) {
    if (matrix_adj[x - 1][y - 1] > z) {

        matrix_adj[x - 1][y - 1] = z;
        previous[x - 1][y - 1] = x - 1;

        matrix_adj[y - 1][x - 1] = z;
        previous[y - 1][x - 1] = y - 1;
    }
}

void find_shortest_paths(int ** matrix_adj, int ** previous) {
    for (int k = 0; k < point_count; k++) {
        for (int i = 0; i < point_count; i++) {
            for (int j = 0; j < point_count; j++) {
                if (matrix_adj[i][k] + matrix_adj[k][j] < matrix_adj[i][j]) {
                    matrix_adj[i][j] = matrix_adj[i][k] + matrix_adj[k][j];
                    previous[i][j] = previous[k][j];
                }
            }
        }
    }
}

void add_to_path(Path * path, int x) {
    path->set[path->len] = x;
    path->len++;
}

void get_path(int ** previous, Path * path, int from, int to) {
    path->len = 0;

    int s = from;
    int f = to;
    int current_point = f;
    while (current_point != s) {
        add_to_path(path, current_point + 1);
        current_point = previous[s][current_point];
    }
    add_to_path(path, current_point + 1);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d %d %d %d\n", &point_count, &ways_count,
          &request_path_count, &request_time_count);

    int ** matrix_adjacencies = create_table(point_count, point_count);
    int ** previous = create_table(point_count, point_count);

    int first_point, second_point, time;
    for (int i = 0; i < ways_count; i++) {
        scanf("%d %d %d\n", &first_point, &second_point, &time);
        add_to_matrix_adj(matrix_adjacencies, previous, first_point, second_point, time);
    }

    find_shortest_paths(matrix_adjacencies, previous);

    /* Requests */
        // time with a path
    Path * path = create_path(point_count);
    for (int i = 0; i < request_path_count; i++) {
        scanf("%d %d\n", &first_point, &second_point);
        printf("%d ", matrix_adjacencies[first_point - 1][second_point - 1]);

        get_path(previous, path, first_point - 1, second_point - 1);

        printf("%d ", path->len);
        for (int j = path->len - 1; j >= 0; j--) {
            printf("%d ", path->set[j]);
        }
        printf("\n");
    }
        // only time
    for (int i = 0; i < request_time_count; i++) {
        scanf("%d %d\n", &first_point, &second_point);
        printf("%d\n", matrix_adjacencies[first_point - 1][second_point - 1]);
    }

    /* Clearning the memory */
    for (int i = 0; i < point_count; i++)
        free(matrix_adjacencies[i]);
    free(matrix_adjacencies);

    for (int i = 0; i < point_count; i++)
        free(previous[i]);
    free(previous);

    free(path->set);
    free(path);

    return 0;
}