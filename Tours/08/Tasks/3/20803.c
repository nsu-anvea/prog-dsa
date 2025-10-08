#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct coord {
	int x;
	int y;
};

typedef struct city {
    struct coord coord;
} City;

typedef struct highway {
    double distance;
    int first_city;
    int second_city;
} Highway;

typedef struct coned {
    int            city_num;
    struct coned * next;
} Connected;

typedef struct list {
    struct coned * first;
} List;

typedef struct heap {
    int len;
    struct highway * set;
} Heap;

struct city * cities;

void swap_int(int * a, int * b)
{
    int c = * a;
    * a = * b;
    * b = c;
}

void swap_double(double * a, double * b)
{
    double c = * a;
    * a = * b;
    * b = c;
}

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return (2 * i + 1);
}

int right(int i) {
    return (2 * i + 2);
}

void sift_up(Heap * H, int i)
{
    if (!i)
        return;
    int p = parent(i);
    if (H->set[i].distance > H->set[p].distance) {
        swap_double(&H->set[i].distance, &H->set[p].distance);
        swap_int(&H->set[i].first_city, &H->set[p].first_city);
        swap_int(&H->set[i].second_city, &H->set[p].second_city);
        sift_up(H, p);
    }
}

void sift_down(Heap * H, int i)
{
    int l = left(i);
    int r = right(i);
    int t;

    /* H[i] < H[l] -> переход к t = l */
    if (l < H->len && H->set[l].distance > H->set[i].distance) {
        t = l;
    } else
        t = i;

    if (r < H->len && H->set[r].distance > H->set[t].distance)
        t = r;
    if (t != i) {
        swap_double(&H->set[i].distance, &H->set[t].distance);
        swap_int(&H->set[i].first_city, &H->set[t].first_city);
        swap_int(&H->set[i].second_city, &H->set[t].second_city);
        sift_down(H, t);
    }
}

void push_to_heap(Heap * H, int city_1, int city_2)
{
    H->set[H->len].distance = pow(cities[city_1].coord.x - cities[city_2].coord.x, 2) +
            pow(cities[city_1].coord.y - cities[city_2].coord.y, 2);
    H->set[H->len].first_city = city_1;
    H->set[H->len].second_city = city_2;

    sift_up(H, H->len);
    H->len++;
}

double pop_from_heap(Heap * H)
{
    double del_item = H->set[0].distance;

    swap_double(&H->set[0].distance, &H->set[H->len - 1].distance);
    swap_int(&H->set[0].first_city, &H->set[H->len - 1].first_city);
    swap_int(&H->set[0].second_city, &H->set[H->len - 1].second_city);

    H->len--;
    sift_down(H, 0);

    return del_item;
}

void build_heap(Heap * H, int start) {
    for (int i = start; i >= 0; i--)
        sift_down(H, i);
}

void sort_on_the_heap(Heap * H)
{
    int len = H->len;
    for (int i = len - 1; i > 0; i--)
    {
        swap_double(&H->set[i].distance, &H->set[0].distance);
        swap_int(&H->set[i].first_city, &H->set[0].first_city);
        swap_int(&H->set[i].second_city, &H->set[0].second_city);

        H->len--;
        sift_down(H, 0);
    }
    H->len = len;
}

void heap_sort(Heap * H)
{
    int middle = (H->len - 1) / 2;
    build_heap(H, middle);
    sort_on_the_heap(H);
}

Heap * create_heap(int size) {
    Heap * H = (Heap *)malloc(sizeof(Heap));
    H->len = 0;
    H->set = (Highway *)malloc(sizeof(Highway) * size);
    return H;
}

City * create_coord_cities(int size)
{
    City * S = (City *)malloc(sizeof(City) * size);
    return S;
}

List * create_adjacencies_list(int size)
{
    List * adj_list = (List *)malloc(sizeof(List) * size);
    for (int i = 0; i < size; i++)
        adj_list[i].first = NULL;
    return adj_list;
}

int * get_mem_int(int size)
{
    int * S = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
        S[i] = 0;
    return S;
}

void add_edge(List * adj_list, int x, int y)
{
    Connected * current = adj_list[x - 1].first;
    Connected * previous = NULL;

    int is_writed = 0;
    while (current)
    {
        if (current->city_num == y - 1) {
            is_writed = 1;
            break;
        }
        previous = current;
        current = previous->next;
    }
    if (!is_writed) {
        Connected * new_item = (Connected *) malloc(sizeof(Connected));
        new_item->city_num = y - 1;
        new_item->next = NULL;

        if (!previous)
            adj_list[x - 1].first = new_item;
        else
            previous->next = new_item;
    }
}

void make_set(int * marks, int * rank, int x)
{
    marks[x] = x;
    rank[x] = 0;
}

int find_set(int * marks, int x)
{
    if (x == marks[x])
        return x;
    return marks[x] = find_set(marks, marks[x]);
}

void union_set(int * marks, int * rank, int x, int y)
{
    x = find_set(marks, x);
    y = find_set(marks, y);

    if (x == y)
        return;
    if (rank[x] > rank[y])
        marks[y] = x;
    else {
        marks[x] = y;
        if (rank[x] == rank[y])
            ++rank[y];
    }
}

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int city_count;
	scanf("%d\n", &city_count);

    cities = create_coord_cities(city_count);

    int x, y;
    for (int i = 0; i < city_count; i++)
    {
        scanf("%d %d\n", &x, &y);

        cities[i].coord.x = x;
        cities[i].coord.y = y;
    }

	int highways_count;
	scanf("%d\n", &highways_count);

    List * adjacencies = create_adjacencies_list(city_count);
    Heap * distances = create_heap(city_count * city_count);

    int * marks = get_mem_int(city_count);
    int * rank = get_mem_int(city_count);
    for (int i = 0; i < city_count; i++)
        make_set(marks, rank, i);

	int first_city, second_city;
	for (int i = 0; i < highways_count; i++)
	{
		scanf("%d %d\n", &first_city, &second_city);
		add_edge(adjacencies, first_city, second_city);
        add_edge(adjacencies, second_city, first_city);
        union_set(marks, rank, first_city - 1, second_city - 1);
	}

    for (int i = 0; i < city_count; i++)
        for (int j = i + 1; j < city_count; j++)
            if (find_set(marks, i) != find_set(marks, j))
                push_to_heap(distances, i, j);
    heap_sort(distances);

    for (int i = 0; i < distances->len; i++) {
        Highway * highway = &distances->set[i];
        if (find_set(marks, highway->first_city) != find_set(marks, highway->second_city)) {
            union_set(marks, rank, highway->first_city, highway->second_city);
            add_edge(adjacencies, highway->first_city + 1, highway->second_city + 1);
            add_edge(adjacencies, highway->second_city + 1, highway->first_city + 1);
            printf("%d %d\n", highway->first_city + 1, highway->second_city + 1);
        }
    }

	/* Clearning the memory */

    free(cities);
    free(distances);

    Connected * current;
    Connected * previous;
    for (int i = 0; i < city_count; i++)
    {
        current = adjacencies[i].first;
        while (current)
        {
            previous = current;
            current = previous->next;
            free(previous);
        }
    }
    free(adjacencies);
    free(marks);
    free(rank);

	return 0;
}