#include <stdio.h>
#include <stdlib.h>
#define INF 1000000000
#define VERTICES 5000
#define EDGES 50000
#define FRIENDS 50

int vertices_count;
int edges_count;
int friends_count;

typedef struct edge {
	int from;
	int to;
	int weight;
} Edge;

Edge * create_edges() {
	Edge * S = (Edge *)malloc(sizeof(Edge) * edges_count);
    int from, to, weight;
    for (int i = 0; i < edges_count; i++) {
        scanf("%d %d %d\n", &from, &to, &weight);
        S[i].from = from - 1;
        S[i].to = to - 1;
        S[i].weight = weight;
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
    putchar('\n');
}

void bellman_ford(Edge * edges, int * dist, int * prev, int start) {
	for (int i = 0; i < vertices_count; i++)
		dist[i] = INF;
	dist[start] = 0;

    int from, to, weight;
	for (int i = 0; i < vertices_count - 1; i++) {
        for (int j = 0; j < edges_count; j++) {
            from = edges[j].from;
            to = edges[j].to;
            weight = edges[j].weight;
            if (dist[from] != INF && dist[to] > dist[from] + weight) {
                dist[to] = dist[from] + weight;
                prev[to] = from;
            }
        }
    }
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	scanf("%d %d %d\n", &vertices_count, &edges_count, &friends_count);

	int friends[FRIENDS];
	for (int i = 0; i < friends_count; i++) {
		scanf("%d", &friends[i]);
		friends[i]--;
	}

	Edge * edges = create_edges();

    int previous[VERTICES];
    int dist[VERTICES];
    int path[VERTICES];
    for (int j = 0; j < vertices_count; j++)
        previous[j] = -1;

    int start = 0;
	bellman_ford(edges, dist, previous, start);

    for (int i = 0; i < friends_count; i++) {
        printf("%d ", dist[friends[i]]);
        get_path(path, previous, friends[i]);
    }

    /* Clearning the memory */
    free(edges);

	return 0;
}