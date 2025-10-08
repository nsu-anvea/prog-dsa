#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000001
#define nemax 10

typedef struct list {
    char word[10];
} UUG;

typedef struct LIST {
    int size;
    int wrote;
    struct list * mas;
} UG;

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N;
    scanf("%d", &N);

    UG * General = (UG *)malloc( sizeof(UG) * MAX );
    for (int i = 0; i < MAX; i++) {
        General[i].size = 10;
        General[i].wrote = 0;
        General[i].mas = (UUG *)malloc( sizeof(UUG) * General[i].size );
    }

    int key, word[10];
    for (int i = 0; i < N; i++) {
        scanf("%d ", &key);

        if (General[key].wrote + 1 > General[key].size) {
            General[key].mas = (UUG *)realloc( General[key].mas, sizeof(UUG) * 2 * General[key].size );
            General[key].size *= 2;
        }
        scanf("%s", General[key].mas[General[key].wrote].word);
        General[key].wrote++;
    }

    int cnt;
    for (int k = 0; k < MAX; k++) {
        if (General[k].wrote != 0) {
            cnt = 1;
            while (cnt <= General[k].wrote) {
                printf("%d %s\n", k, General[k].mas[cnt - 1].word);
                cnt++;
            }
        }
    }

    // Clearning the memory
    for (int i = 0; i < MAX; i++) {
        free(General[i].mas);
    }
    free(General);

    return 0;
}