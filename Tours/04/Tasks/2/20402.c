#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SN 9785021

typedef struct bio {
    int          cnt;
    int          year;
    char         country[15];
    struct bio * next;
} Bio;

typedef struct node {
    int           len;
    char          name[35];
    struct bio  * first;
    struct node * left;
    struct node * right;
} tree;

tree * createTree(char * name, Bio ** bio) {
    tree * T = (tree *)malloc(sizeof(tree));
    T->len = 1; // Кол-во

    int i = 0;
    while (name[i] != '\0') { // Имя
        T->name[i] = name[i];
        i++;
    }
    T->name[i] = '\0';

    T->first = (* bio);

    T->first->cnt = 1;

    T->left = NULL;
    T->right = NULL;

    return T;
}

void clearning(tree * t) {
    if (!t) return;
    clearning(t->left);
    clearning(t->right);
    free(t);
}

int hash(char* s) {
    unsigned int hash = 0;
    for (int i = 0; s[i]; i++)
    {
        hash += s[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % SN;
}


int compare(char * first, char * second) {
    int h1 = hash(first);
    int h2 = hash(second);

    if (h2 < h1) return -1;
    else if (h2 > h1) return 1;
    else {
        int j = 0;
        while (second[j] != '\0' && first[j] != '\0') {
            if (second[j] < first[j]) {
                return -1;
            }
            if (second[j] > first[j]) {
                return 1;
            }
            j++;
        }
        if (second[j] == '\0' && first[j] != '\0') return -1;
        if (second[j] != '\0' && first[j] == '\0') return 1;
    }
    return 0;
}

tree * add(tree * t, char * name, Bio ** bio) {
    if (!t) {
        t = createTree(name, bio);
        return t;
    }
    int cmp = compare(t->name, name);
    if      (cmp == -1) t->left = add(t->left, name, bio);    // -1 => new bio < bio_t
    else if (cmp == 1) t->right = add(t->right, name, bio);   // 1 => new bio > bio_t
    else {
        Bio * temp = t->first;    // 0 => new bio = bio_t
        Bio * prev;
        int isWrite = 0;
        for (int i = 0; i < t->len; i++) {
            if (temp->year == (* bio)->year) {
                if (!compare(temp->country, (* bio)->country)) {
                    temp->cnt++;
                    isWrite = 1;
                    break;
                }
            }
            prev = temp;
            temp = temp->next;
        }
        if (!isWrite) {
            t->len++;
            prev->next = (* bio);
        }
    }
    return t;
}

void findByName(tree * t, char * name, char * movie) {
    if (!t) return;

    // Сравнить имя
    int cmp = compare(t->name, name);

    if (cmp == -1) findByName(t->left, name, movie);
    else if (cmp == 1) findByName(t->right, name, movie);
    else {
        Bio * temp = t->first;
        for (int i = 0; i < t->len; i++) {
            printf("\"%s\" %d \"%s\" \"%s\" \"%s\"\n", t->name, temp->year, temp->country, name, movie);
            temp = temp->next;
        }
    }
}

void getNewBio(char * name, Bio ** bio) {
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

    scanf(" %d \"", &(* bio)->year); // get a year

    scanf("%c", &x);
    l = 0;
    while (x != '\"') { // get a country
        (* bio)->country[l] = x;
        l++;
        scanf("%c", &x);
    }
    (* bio)->country[l] = '\0';
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

void execute(tree * S) {
    int N;
    scanf("%d\n", &N);

    char name[35];

    for (int i = 0; i < N; i++) {
        Bio * bio = (Bio *)malloc(sizeof(Bio));
        bio->cnt = 1;
        bio->next = NULL;
        getNewBio(name, &bio);
        S = add(S, name, &bio);
    }

    int M;
    scanf("%d\n", &M);

    char film[25];

    for (int i = 0; i < M; i++) {
        getNewMovie(name, film);
        findByName(S, name, film);
    }
    // Clearning the memory
    clearning(S);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    tree * Tree = NULL;

    execute(Tree);

    return 0;
}