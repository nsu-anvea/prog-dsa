#include <stdio.h>
#include <stdlib.h>
#define STYPE int

typedef struct stack_item {
    STYPE               SIe;
    struct stack_item * next;
} Stack_Item;
typedef struct stack {
    struct stack_item * top;
} Stack;

int empty(Stack * stack) {
    return stack->top == NULL;
}

STYPE top(Stack * S) {
    return S->top->SIe;
}

void push(Stack * S, STYPE SIe) {
    Stack_Item * n = (Stack_Item *)malloc(sizeof(Stack_Item));
    n->SIe = SIe;
    n->next = S->top;

    S->top = n;
}

STYPE pop(Stack * S) {
    Stack_Item * d = S->top;

    S->top = d->next;

    int SIe;
    SIe = d->SIe;

    free(d);
    return SIe;
}

Stack * create_stack() {
    Stack * S = (Stack *)malloc(sizeof(Stack));
    S->top = NULL;
    return S;
}

int ** create_adj_matrix(int size) {
    int ** adj_matrix = (int **)calloc(size, sizeof(int *));
    for (int i = 0; i < size; i++)
        adj_matrix[i] = (int *) calloc(size, sizeof(int));
    return adj_matrix;
}

void dfs(int ** adj_matrix, int * visited, int vertices_count, int v) {
    visited[v] = 1;
    for (int u = 0; u < vertices_count; u++) {
        if (adj_matrix[v][u] && !visited[u]) {
            dfs(adj_matrix, visited, vertices_count, u);
        }
    }
}

int check_for_Euler_path(int ** adj_matrix, int * deg, int vertices_count) {
    int odd_vertex = 0;
    for (int v = 0; v < vertices_count; v++)
        if (deg[v] % 2 != 0)
            odd_vertex++;
    if (odd_vertex > 2)
        return 0;

    int * visited = (int *)calloc(vertices_count, sizeof(int));
    for (int v = 0; v < vertices_count; v++) {
        if (deg[v] > 0) {
            dfs(adj_matrix, visited, vertices_count, v);
            break;
        }
    }
    for (int v = 0; v < vertices_count; v++)
        if (deg[v] > 0 && !visited[v]) {
            free(visited);
            return 0;
        }
    free(visited);
    return 1;
}

void find_Euler_path(int ** adj_matrix, int * deg, int vertices_count, int v) {
    int * path = (int *)calloc(vertices_count * vertices_count, sizeof(int));
    int   path_len = 0;

    for (int u = 0; u < vertices_count; u++) {
        if (deg[u] % 2 != 0) {
            v = u;
            break;
        }
    }
    Stack * stack = create_stack();
    push(stack, v);

    while (!empty(stack)) {
        int w = top(stack);
        int found_edge = 0;
        for (int u = 0; u < vertices_count; u++) {
            if (adj_matrix[w][u] > 0) {
                push(stack, u);
                adj_matrix[w][u] *= -1;
                adj_matrix[u][w] *= -1;
                found_edge = 1;
                break;
            }
        }
        if (!found_edge) {
            pop(stack);
            path[path_len++] = w;
        }
    }
    for (int u = 1; u < path_len; u++) {
        printf("%d\n", -1 * adj_matrix[path[u - 1]][path[u]]);
    }

    free(stack);
    free(path);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int vertices_count, edges_count;
    scanf("%d %d\n", &vertices_count, &edges_count);

    int ** adj_matrix = create_adj_matrix(vertices_count);
    int  * deg = (int *)calloc(vertices_count, sizeof(int));

    int v1, v2;
    for (int i = 0; i < edges_count; i++) {
        scanf("%d %d\n", &v1, &v2);
        deg[v1 - 1]++;
        deg[v2 - 1]++;
        adj_matrix[v1 - 1][v2 - 1] = i + 1;
        adj_matrix[v2 - 1][v1 - 1] = i + 1;
    }

    if (!check_for_Euler_path(adj_matrix, deg, vertices_count)) {
        printf("NO\n");
    }
    else {
        printf("YES\n");
        for (int v = 0; v < vertices_count; v++) {
            if (deg[v] > 0) {
                find_Euler_path(adj_matrix, deg, vertices_count, v);
                break;
            }
        }
    }

    /* Clearning the memory */
    for (int i = 0; i < vertices_count; i++) {
        free(adj_matrix[i]);
    }
    free(adj_matrix);
    free(deg);

    return 0;
}