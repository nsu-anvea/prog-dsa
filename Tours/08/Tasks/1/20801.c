#include <stdio.h>
#include <stdlib.h>

typedef struct coned {
    int            num_vetrex;
    struct coned * next;
} Connected;

typedef struct list {
    int            visited;
    int            path;
    struct coned * first;
} List;

typedef struct el {
    int         num_vertex;
    int         path_len;
    struct el * next;
} Element;

typedef struct queue {
    int         len;
    struct el * first;
    struct el * end;
} Queue;

int lenght(Queue * S) {
    return S->len;
}

int vertex_first(Queue * S) {
    return S->first->num_vertex;
}

int path_first(Queue * S) {
    return S->first->path_len;
}

void enqueue(Queue * S, int vertex, int path)
{
    Element * new = (Element *)malloc(sizeof(Element));
    new->num_vertex = vertex;
    new->path_len = path;
    new->next = NULL;

    if (S->len != 0) {
        S->end->next = new;
        S->end = new;
        S->len++;
    } else {
        S->first = new;
        S->end = new;
        S->len++;
    }
}

int dequeue(Queue * S)
{
    Element * Deleted = S->first;
    S->first = Deleted->next;
    S->len--;

    int vertex;
    vertex = Deleted->num_vertex;

    free(Deleted);
    return vertex;
}

Queue * create_queue()
{
    Queue * queue = (Queue *)malloc(sizeof(Queue));
    queue->len = 0;
    queue->first = NULL;
    queue->end = NULL;
    return queue;
}

void add_edge(List * adj_list, int x, int y)
{
    Connected * current = adj_list[x - 1].first;
    Connected * previous = NULL;
    int is_writed = 0;

    while (current)
    {
        if (current->num_vetrex == y - 1) {
            is_writed = 1;
            break;
        }
        previous = current;
        current = previous->next;
    }
    if (!is_writed) {
        Connected * new = (Connected *) malloc(sizeof(Connected));
        new->num_vetrex = y - 1;
        new->next = NULL;
        if (!previous)
            adj_list[x - 1].first = new;
        else previous->next = new;
    }
}

List * create_adjacencies_list(int size)
{
    List * adj_list = (List *)malloc(sizeof(List) * size);
    for (int i = 0; i < size; i++) {
        adj_list[i].visited = 0;
        adj_list[i].path = 0;
        adj_list[i].first = NULL;
    }
    return adj_list;
}

void BFS(List * adj_list)
{
    Queue * queue = create_queue();
    enqueue(queue, 0, 0);

    adj_list[0].visited = 1;

    while (lenght(queue))
    {
        Connected * current;
        current = adj_list[vertex_first(queue)].first;
        adj_list[vertex_first(queue)].path = path_first(queue);
        while (current)
        {
            if (!adj_list[current->num_vetrex].visited) {
                adj_list[current->num_vetrex].visited = 1;
                enqueue(queue, current->num_vetrex, path_first(queue) + 1);
            }
            current = current->next;
        }
        dequeue(queue);
    }
    free(queue);
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int count_vertices;
    int count_edges;
    scanf("%d %d\n", &count_vertices, &count_edges);

    List * adjacencies = create_adjacencies_list(count_vertices);

    int _1_vertex, _2_vertex;
    for (int i = 0; i < count_edges; i++)
    {
        scanf("%d %d\n", &_1_vertex, &_2_vertex);
        add_edge(adjacencies, _1_vertex, _2_vertex);
    }

    /* Finding the shortest path to all vertices */
    BFS(adjacencies);

    for (int i = 0; i < count_vertices; i++)
    {
        if (adjacencies[i].visited)
            printf("%d\n", adjacencies[i].path);
        else puts("-1");
    }

    /* Clearning the memory */
    for (int i = 0; i < count_vertices; i++) {
        Connected * current = adjacencies[i].first;
        Connected * previous;
        while (current)
        {
            previous = current;
            current = previous->next;
            free(previous);
        }
    }
    free(adjacencies);

    return 0;
}