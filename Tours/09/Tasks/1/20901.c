#include <stdio.h>
#include <stdlib.h>
#define MAXINT 2000000000

int serverCount;
int sender, recipient;

typedef struct node {
    int           server;
    int 		  time;
    struct node * next;
} Node;

typedef struct list {
    struct node * first;
} List;

void addChannel(List * channels, int x, int y, int time)
{
    Node * current = (Node *) channels[x - 1].first;
    Node * previous = NULL;

    int isWrited = 0;
    while (current)
    {
        if (current->server == y - 1) {
            isWrited = 1;
            break;
        }
        previous = current;
        current = previous->next;
    }
    if (!isWrited) {
        Node * newItem = (Node *)malloc(sizeof(Node));
        newItem->server = y - 1;
        newItem->time = time;
        newItem->next = NULL;

        if (!previous)
            channels[x - 1].first = newItem;
        else
            previous->next = newItem;
    }
}

List * createChannelsList(int size)
{
    List * channels = (List *)malloc(sizeof(List) * size);
    for (int i = 0; i < size; i++)
        channels[i].first = NULL;
    return channels;
}

int * getMemInt(int size)
{
    int * S = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
        S[i] = 0;
    return S;
}

int ** createTable(int size)
{
    int ** S = (int **)malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++)
    {
        S[i] = (int *)malloc(sizeof(int) * size);
        for (int j = 0; j < size; j++)
            S[i][j] = MAXINT;
    }
    return S;
}

int findShortestTime(List * channels, int ** table, int * selected)
{
    int shortestTime;

    int currentServer = sender - 1;
    int currentTime = 0;

    /* Set initial values */
    selected[currentServer] = 1;
    table[0][currentServer] = currentTime;

    Node * current = channels[currentServer].first;
    while (current)
    {
        table[0][current->server] = current->time;
        current = current->next;
    }

    /* The general algorithm */
    for (int i = 1; i < serverCount; i++)
    {
        currentTime = MAXINT;
        for (int j = 0; j < serverCount; j++)
        {
            if (!selected[j]) {
                if (table[i - 1][j] < currentTime) {
                    currentServer = j;
                    currentTime = table[i - 1][j];
                }
            }
            table[i][j] = table[i - 1][j];
        }
        selected[currentServer] = 1;

        current = channels[currentServer].first;
        while (current)
        {
            if (!selected[current->server]) {
                if (current->time + currentTime < table[i][current->server]) {
                    table[i][current->server] = current->time + currentTime;
                }
            }
            current = current->next;
        }
    }
    shortestTime = table[serverCount - 1][recipient - 1];

    if (shortestTime != MAXINT)
        return shortestTime;
    return -1;
}

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	scanf("%d\n", &serverCount);
	scanf("%d %d\n", &sender, &recipient);

	List * channels = createChannelsList(serverCount);
    int * selected = getMemInt(serverCount);
    int ** table = createTable(serverCount);

	int firstServer, secondServer, time;
	while (scanf("%d %d %d\n", &firstServer, &secondServer, &time) != -1)
    {
		addChannel(channels, firstServer, secondServer, time);
        //addChannel(channels, secondServer, firstServer, time);
    }

    int shortestTime = findShortestTime(channels, table, selected);
    if (shortestTime != -1)
        printf("%d\n", shortestTime);
    else
        printf("no\n");

    for (int i = 0; i < serverCount; i++) {
        for (int j = 0; j < serverCount; j++) {

        }
    }

    /* Clearning the memory */
    Node * current;
    Node * previous;
    for (int i = 0; i < serverCount; i++)
    {
        current = channels[i].first;
        while (current)
        {
            previous = current;
            current = previous->next;
            free(previous);
        }
    }
    free(channels);

    for (int i = 0; i < serverCount; i++)
        free(table[i]);
    free(table);

    free(selected);

	return 0;
}