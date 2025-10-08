#include <stdio.h>
#include <stdlib.h>

char ** create(int size) {
    char ** S = (char **)malloc(sizeof(char *) * size);
    char skip;
    for (int i = 0; i < size; i++) {
        S[i] = (char *)malloc(sizeof(char) * size);
        for (int j = 0; j < size; j++) {
            scanf("%c", &S[i][j]);
        }
        scanf("%c", &skip);
    }
    return S;
}

void dfs(char ** maze, int N, int X, int Y) {
    maze[Y][X] = '*';
    // Check for 3 parties
    if (X > 0 && maze[Y][X - 1] == ' ') // left
        dfs(maze, N, X - 1, Y);

    if (X < N - 1 && maze[Y][X + 1] == ' ') // right;
        dfs(maze, N, X + 1, Y);

    if (Y > 0 && maze[Y - 1][X] == ' ') // up
        dfs(maze, N, X, Y - 1);

    if (Y < N - 1 && maze[Y + 1][X] == ' ') // down
        dfs(maze, N, X, Y + 1);
}

void execute(int N) {
    char ** maze = create(N); // maze - лабиринт

    // Search for an entrance
    int startX = 0;
    int startY = 0;
    for (int i = 0; i < N; i++) if (maze[0][i] == ' ') {
        startX = i;
        break;
    }
    dfs(maze, N, startX, startY); // Try to visit all places

    int cntSecretRooms = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (maze[i][j] == ' ') {
                dfs(maze, N, j, i);
                cntSecretRooms++;
            }
        }
    }

    printf("%d\n", cntSecretRooms);

    // Clearning the memory
    for (int i = 0; i < N; i++) {
        free(maze[i]);
    }
    free(maze);
}

int main() {
    freopen("input.txt", "r", stdin);

    int N;
    scanf("%d\n", &N);

    execute(N);

    return 0;
}