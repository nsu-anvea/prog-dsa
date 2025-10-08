#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SN 9785021

typedef struct bio {
    int cnt;

    char name[35];
    int  year;
    char country[15];

    struct bio * next;
} Bio;

typedef struct list {
    int len;
    struct bio * first;
} List;

List * create() {
    List * S = (List *)malloc(sizeof(List) * SN);
    for (int i = 0; i < SN; i++) {
        S[i].len = 0;
        S[i].first = NULL;
    }
    return S;
}

unsigned int hash(char * str) {
    unsigned int h = 0;

    for (int i = 0; str[i]; i++) {
        h += str[i];
        h += (h << 10);
        h ^= (h >> 6);
    }
    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h % SN;
}

void add(List * S, char * name, int year, char * country) {
    unsigned int hash_t = hash(name);

    if (!S[hash_t].first) {
        Bio * A = (Bio *)malloc(sizeof(Bio));

        A->cnt = 1; // Кол-во

        int i = 0;
        while (name[i] != '\0') { // Имя
            A->name[i] = name[i];
            i++;
        }
        A->name[i] = '\0';

        A->year = year; // Год

        i = 0;
        while (country[i] != '\0') { // Страна
            A->country[i] = country[i];
            i++;
        }
        A->country[i] = '\0';

        S[hash_t].first = A;
        S[hash_t].len++;
    }

    else {
        int isWrite = 0;

        Bio * t = S[hash_t].first;
        Bio * p;
        for (int i = 0; i < S[hash_t].len; i++) {
            // Сравнить год
            int isEqualYear = 0;
            if (year == t->year) isEqualYear = 1;

            if (isEqualYear) {
                // Сравнить имя
                int j = 0, isEqualName = 1;
                while (name[j] != '\0' && t->name[j] != '\0') {
                    if (name[j] != t->name[j]) isEqualName = 0;
                    j++;
                }
                if (isEqualName) if (
                        (name[j] == '\0' && t->name[j] != '\0') ||
                        (name[j] != '\0' && t->name[j] == '\0')
                        ) isEqualName = 0;

                if (isEqualName) {
                    // Сравнить страну
                    int isEqualCountry = 1;
                    j = 0;
                    while (country[j] != '\0' && t->country[j] != '\0') {
                        if (country[j] != t->country[j]) isEqualCountry = 0;
                        j++;
                    }
                    if (isEqualCountry) if (
                            (country[j] == '\0' && t->country[j] != '\0') ||
                            (country[j] != '\0' && t->country[j] == '\0')
                            ) isEqualCountry = 0;

                    // Если bio одинаковое
                    if (isEqualCountry) {
                        t->cnt++;
                        isWrite = 1;
                        break;
                    }
                }
            }
            p = t;
            t = t->next;
        }
        if (!isWrite) {
            Bio * A = (Bio *)malloc(sizeof(Bio));

            A->cnt = 1; // Кол-во

            int z = 0;
            while (name[z] != '\0') { // Имя
                A->name[z] = name[z];
                z++;
            }
            A->name[z] = '\0';

            A->year = year; // Год

            z = 0;
            while (country[z] != '\0') { // Страна
                A->country[z] = country[z];
                z++;
            }
            A->country[z] = '\0';

            p->next = A;
            S[hash_t].len++;
        }
    }
}

void findByName(List * S, char * name, char * movie) {
    unsigned int hash_t = hash(name);

    if (S[hash_t].first) {
        Bio * t = S[hash_t].first;
        int isEqualName;
        for (int i = 0; i < S[hash_t].len; i++) {
            // Сравнить имя
            isEqualName = 1;
            int j = 0;
            while (name[j] != '\0' && t->name[j] != '\0') {
                if (name[j] != t->name[j]) isEqualName = 0;
                j++;
            }
            if (isEqualName) if (
                        (name[j] == '\0' && t->name[j] != '\0') ||
                        (name[j] != '\0' && t->name[j] == '\0')
                        ) isEqualName = 0;

            if (isEqualName) {
                for (int z = 0; z < t->cnt; z++) {
                    printf("\"%s\" %d \"%s\" \"%s\" \"%s\"\n", t->name, t->year, t->country, name, movie);
                }
            }
            t = t->next;
        }
    }
}

void getNewBio(char * name, int * year, char * country) {
    char x;
    scanf("%c", &x); // skip

    scanf("%c", &x);
    int l = 0;
    while (x != '\"') { // get a name
        name[l] = x;
        l++;
        scanf("%c", &x);
    }
    name[l] = '\0';

    scanf(" %d \"", year); // get a year

    scanf("%c", &x);
    l = 0;
    while (x != '\"') { // get a country
        country[l] = x;
        l++;
        scanf("%c", &x);
    }
    country[l] = '\0';
    scanf("%c", &x); // skip \n
}

void getNewMovie(char * name, char * film) {
    char x;
    scanf("%c", &x); // skip

    scanf("%c", &x);
    int l = 0;
    while (x != '\"') { // get a name
        name[l] = x;
        l++;
        scanf("%c", &x);
    }
    name[l] = '\0';

    scanf("%c", &x); // skip
    scanf("%c", &x); // skip

    scanf("%c", &x);
    l = 0;
    while (x != '\"') { // get a country
        film[l] = x;
        l++;
        scanf("%c", &x);
    }
    film[l] = '\0';
    scanf("%c", &x); // skip \n
}

void execute() {
    List * hashTable = create();

    int N;
    scanf("%d\n", &N);

    char name[35];
    int  year;
    char country[15];

    for (int i = 0; i < N; i++) {
        getNewBio(name, &year, country);
        add(hashTable, name, year, country);
    }

    int M;
    scanf("%d\n", &M);

    char film[25];

    for (int i = 0; i < M; i++) {
        getNewMovie(name, film);
        findByName(hashTable, name, film);
    }

    // Clearning the memory
    Bio * t, * n;
    for (int i = 0; i < SN; i++) {
        t = hashTable[i].first;
        for (int j = 0; j < hashTable[i].len; j++) {
            n = t->next;
            free(t);

            t = n;
        }
    }
    free(hashTable);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    execute();

    return 0;
}