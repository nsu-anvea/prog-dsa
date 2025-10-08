#include <stdio.h>
#include <stdlib.h>
#define MAX 3000
#define INF 1500000000

typedef struct pair {
    int a;
    int b;
} Pair;

typedef struct node {
    int           home;
    int 		  time;
    struct node * next;
} Node;

typedef struct list {
    struct node * first;
} List;

int * get_mem_int(int size) {
    int * S = (int *)malloc(sizeof(int) * size);
    return S;
}

Pair * create_requests_int(int size) {
    Pair * S = (Pair *)malloc(sizeof(Pair) * size);
    for (int i = 0; i < size; i++)
        scanf("%d %d\n", &S[i].a, &S[i].b);
    return S;
}

int ** create_adj_int(int size) {
    int ** S = (int **)malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++) {
        S[i] = (int *)malloc(sizeof(int) * size);
        for (int j = 0; j < size; j++) {
            if (i == j)
                S[i][j] = 0;
            else
                S[i][j] = INF;
        }
    }
    return S;
}

void get_path(int * path, int * previous, int finish) {
    int len = 0;
    for (int i = finish; i != -1; i = previous[i]) {
        path[len] = i + 1;
        len++;
    }
    printf("%d ", len);
    for (int i = len - 1; i >= 0; i--)
        printf("%d ", path[i]);
    printf("\n");
}

int * find_shortest_time(int ** adj, int * previous, int size, int start, int finish) {
    int time_int[MAX];
    int visited[MAX];
    for (int i = 0; i < size; i++) {
        time_int[i] = INF;
        visited[i] = 0;
    }
    time_int[start] = 0;

    for (int i = 0; i < size; i++) {
        int nearest = -1;
        for (int j = 0; j < size; j++)
            if (!visited[j] && (nearest == -1 || time_int[j] < time_int[nearest]))
                nearest = j;

        if (time_int[nearest] == INF)
            break;

        visited[nearest] = 1;

        for (int j = 0; j < size; j++) {
            if (adj[nearest][j] != INF) {
                if (time_int[j] > time_int[nearest] + adj[nearest][j]) {
                    time_int[j] = time_int[nearest] + adj[nearest][j];
                    previous[j] = nearest;
                }
            }
        }
    }
    return time_int;
}
int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int housewives_count, pairs_count, requests_count;
    scanf("%d %d %d\n", &housewives_count, &pairs_count, &requests_count);

    int ** adjacencies = create_adj_int(housewives_count);
    Pair * requests = create_requests_int(requests_count);

    int first_home, second_home, time;
    for (int i = 0; i < pairs_count; i++) {
        scanf("%d %d %d\n", &first_home, &second_home, &time);
        if (time < adjacencies[first_home - 1][second_home - 1]) {
            adjacencies[first_home - 1][second_home - 1] = time;
            adjacencies[second_home - 1][first_home - 1] = time;
        }
    }

    int previous[MAX];
    int path[MAX];

    for (int i = 0; i < requests_count; i++) {
        for (int j = 0; j < housewives_count; j++)
            previous[j] = -1;

        int * time_int = find_shortest_time(adjacencies, previous,
                                             housewives_count, requests[i].a - 1, requests[i].b - 1);

        if (time_int[requests[i].b - 1] == INF)
            printf("NO\n");
        else {
            printf("YES %d ", time_int[requests[i].b - 1]);
            get_path(path, previous, requests[i].b - 1);
        }
    }

    /* Clearning the memory */
    for (int i = 0; i < housewives_count; i++)
        free(adjacencies[i]);
    free(adjacencies);
    free(requests);

    return 0;
}