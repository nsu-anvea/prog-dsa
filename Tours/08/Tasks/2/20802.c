#include <stdio.h>
#include <stdlib.h>

int lines_count, columns_count;

int start_line, start_column;
int finish_line, finish_column;

typedef struct yach {
    int visited;
    int block;
    int path;
} Yach;

typedef struct el {
    int         line;
    int         column;
    int         path;
    struct el * next;
} Element;

typedef struct queue {
    int         len;
    struct el * first;
    struct el * last;
} Queue;

int lenght(Queue * S) {
    return S->len;
}

int line_first(Queue * S) {
    return S->first->line;
}

int column_first(Queue * S) {
    return S->first->column;
}

int path_first(Queue * S) {
    return S->first->path;
}

void enqueue(Queue * queue, int line, int column, int path)
{
    Element * new = (Element *)malloc(sizeof(Element));
    new->line = line;
    new->column = column;
    new->path = path;
    new->next = NULL;

    if (lenght(queue)) {
        queue->last->next = new;
        queue->last = new;
        queue->len++;
    } else {
        queue->first = new;
        queue->last = new;
        queue->len++;
    }
}

void dequeue(Queue * S)
{
    Element * del = S->first;
    S->first = del->next;
    S->len--;

    free(del);
}

Queue * create_queue()
{
    Queue * queue = (Queue *)malloc(sizeof(Queue));
    queue->len = 0;
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

Yach ** create_table(int lines_cnt, int columns_cnt)
{
    Yach ** table = (Yach **)malloc(sizeof(Yach *) * lines_cnt);

    char cage;
    for (int i = 0; i < lines_cnt; i++)
    {
        table[i] = (Yach *)malloc(sizeof(Yach) * columns_cnt);
        for (int j = 0; j < columns_cnt; j++)
        {
            scanf("%c", &cage);
            table[i][j].visited = 0;
            table[i][j].path = 0;

            if (cage == 'X')
                table[i][j].block = 1;
            else table[i][j].block = 0;

            if (cage == 'S') {
                start_line = i;
                start_column = j;
            }
            else if (cage == 'F') {
                finish_line = i;
                finish_column = j;
            }
        }
        /* skip */
        scanf("%c", &cage);
    }
    return table;
}

void BFS(Yach ** maze)
{
    Queue * queue = create_queue();
    enqueue(queue, start_line, start_column, 0);

    maze[start_line][start_column].visited = 1;

    while (lenght(queue))
    {
        maze[line_first(queue)][column_first(queue)].path = path_first(queue);

        /* left */
        if (column_first(queue) > 0 && !maze[line_first(queue)][column_first(queue)].block) {
            if (!maze[line_first(queue)][column_first(queue) - 1].visited) {
                maze[line_first(queue)][column_first(queue) - 1].visited = 1;
                enqueue(queue, line_first(queue), column_first(queue) - 1, path_first(queue) + 1);
            }
        }
        /* right */
        if (column_first(queue) < columns_count - 1 && !maze[line_first(queue)][column_first(queue)].block) {
            if (!maze[line_first(queue)][column_first(queue) + 1].visited) {
                maze[line_first(queue)][column_first(queue) + 1].visited = 1;
                enqueue(queue, line_first(queue), column_first(queue) + 1, path_first(queue) + 1);
            }
        }
        /* up */
        if (line_first(queue) > 0 && !maze[line_first(queue)][column_first(queue)].block) {
            if (!maze[line_first(queue) - 1][column_first(queue)].visited) {
                maze[line_first(queue) - 1][column_first(queue)].visited = 1;
                enqueue(queue, line_first(queue) - 1, column_first(queue), path_first(queue) + 1);
            }
        }
        /* down */
        if (line_first(queue) < lines_count - 1 && !maze[line_first(queue)][column_first(queue)].block) {
            if (!maze[line_first(queue) + 1][column_first(queue)].visited) {
                maze[line_first(queue) + 1][column_first(queue)].visited = 1;
                enqueue(queue, line_first(queue) + 1, column_first(queue), path_first(queue) + 1);
            }
        }
        dequeue(queue);
    }
    free(queue);
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    scanf("%d %d\n", &lines_count, &columns_count);

    Yach ** maze = create_table(lines_count, columns_count);

    BFS(maze);

    if (maze[finish_line][finish_column].visited)
        printf("%d\n", maze[finish_line][finish_column].path);
    else puts("-1");

    /* Clearning the memory */
    for (int i = 0; i < lines_count; i++)
        free(maze[i]);
    free(maze);

    return 0;
}