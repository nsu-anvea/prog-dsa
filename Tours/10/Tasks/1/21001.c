#include <stdio.h>
#include <stdlib.h>

typedef struct path {
    int   len;
    int * set;
} Path;

typedef struct coned {
    int index_var;
    struct coned * next;
} Connected;

typedef struct list {
    struct coned * first;
} List;

void add_edge(List * adj_list, int x, int y) {
    Connected * a = (Connected *)malloc(sizeof(Connected));
    a->index_var = y - 1;
    a->next = adj_list[x - 1].first;
    adj_list[x - 1].first = a;
}

List * create_adjacencies_list(int size) {
    List * adj_list = (List *)malloc(sizeof(List) * size);
    for (int i = 0; i < size; i++) {
        adj_list[i].first = NULL;
    }
    return adj_list;
}

void add_to_path(Path * path, int var) {
    path->set[path->len] = var;
    path->len++;
}

Path * create_path(int size) {
    Path * path = (Path *)malloc(sizeof(Path));
    path->len = 0;
    path->set = (int *)malloc(sizeof(int) * size);
    return path;
}

int dfs(List * adj_list, int ind_var, int * visited, Path * path) {
    visited[ind_var] = 1;
    add_to_path(path, ind_var + 1);

    Connected * c = adj_list[ind_var].first;
    while (c) {
        if (!visited[c->index_var]) {
            if (dfs(adj_list, c->index_var, visited, path))
                return 1;
        } else if (visited[c->index_var] == 1) {
            int start = 0;
            while (path->set[start] != c->index_var + 1)
                start++;

            printf("%d\n", path->len - start);
            for (int i = start; i < path->len; i++)
                printf("%d ", path->set[i]);
            printf("\n");
            return 1;
        }
        c = c->next;
    }
    path->len--;
    visited[ind_var] = 2;
    return 0;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int variables_count, edges_count;
    scanf("%d %d", &variables_count, &edges_count);

    List * adjacencies = create_adjacencies_list(variables_count);
    Path * path = create_path(variables_count);
    int * visited = (int *)calloc(variables_count, sizeof(int));

    int f_var, s_var;
    for (int i = 0; i < edges_count; i++) {
        scanf("%d %d", &f_var, &s_var);
        add_edge(adjacencies, f_var, s_var);
    }

    for (int i = 0; i < variables_count; i++) {
        if (!visited[i] && dfs(adjacencies, i, visited, path)) {
            free(visited);
            free(path);
            for (int j = 0; j < variables_count; j++) {
                Connected * c = adjacencies[j].first;
                Connected * p;
                while (c) {
                    p = c;
                    c = c->next;
                    free(p);
                }
            }
            free(adjacencies);
            return 0;
        }
    }

    printf("-1");

    free(visited);
    free(path->set);
    free(path);
    for (int i = 0; i < variables_count; i++) {
        Connected *c = adjacencies[i].first;
        Connected *p;
        while (c) {
            p = c;
            c = c->next;
            free(p);
        }
    }
    free(adjacencies);

    return 0;
}
