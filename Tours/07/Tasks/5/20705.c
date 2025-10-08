#include <stdio.h>
#include <stdlib.h>

int order;
int * set_variables;

typedef struct coned {
    int 		   index_var;
    struct coned * next;
} Connected;

typedef struct list {
	int 		   visited;
    struct coned * first;
} List;

void add_edge(List * adj_list, int x, int y)
{
    Connected * current = adj_list[x - 1].first;
    Connected * previous = NULL;
    int is_writed = 0;

    while (current)
    {
        if (current->index_var == y - 1) {
            is_writed = 1;
            break;
        }
        previous = current;
        current = previous->next;
    }
    if (!is_writed) {
        Connected * new = (Connected *) malloc(sizeof(Connected));
        new->index_var = y - 1;
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
        adj_list[i].first = NULL;
    }
    return adj_list;
}

int * get_mem(int size) {
	int * set = (int *)malloc(sizeof(int) * size);
	return set;
}

int dfs(List * adj_list, int ind_var)
{
	int is_not_cycle = 1;
	adj_list[ind_var].visited = 1;

	Connected * current = adj_list[ind_var].first;
	while (current)
	{
		if (adj_list[current->index_var].visited == 1)
			return 0;
		if (!adj_list[current->index_var].visited)
			is_not_cycle = dfs(adj_list, current->index_var);
		current = current->next;
	}
	adj_list[ind_var].visited = 2;
	set_variables[ind_var] = order + 1;
	order--;

	return is_not_cycle;
}

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int variables_count;
	int inequalities_count;
	scanf("%d %d\n", &variables_count, &inequalities_count);

	List * adjacencies = create_adjacencies_list(variables_count);
	set_variables = get_mem(variables_count);
	order = variables_count - 1;

	int _1_num_var, _2_num_var;
	for (int i = 0; i < inequalities_count; i++)
	{
		scanf("%d %d", &_1_num_var, &_2_num_var);
		add_edge(adjacencies, _1_num_var, _2_num_var);
	}

    int is_cycle = 0;
	for (int i = 0; i < variables_count; i++)
	{
		if (adjacencies[i].visited != 2) {
			if (!dfs(adjacencies, i)) {
                puts("NO");
                is_cycle = 1;
                break;
            }
		}
	}
    if (!is_cycle) {
        puts("YES");
        for (int i = 0; i < variables_count; i++)
            printf("%d ", set_variables[i]);
    }

    /* Clearning the memory */
    for (int i = 0 ; i < variables_count; i++)
    {
        Connected * current = adjacencies[i].first;
        Connected * previous;
        while (current)
        {
            previous = current;
            current = current->next;
            free(previous);
        }
    }
    free(adjacencies);
    free(set_variables);

	return 0;
}