#include "fdf_graph.h"
#include <stdio.h>

int is_vertex_valid(t_graph *graph, int vertex_id)
{
	if (graph->vertex[vertex_id])
        return (TRUE);
    return (FALSE);
}

t_graph* create_graph(int max_vertex_count)
{
    t_graph* tmp = ft_calloc(1, sizeof(t_graph));
	if (!tmp)
	    return 0;
	tmp->max_vertex_count = max_vertex_count;
	tmp->current_vertex_count = 0;
	//tmp->graphType = GRAPH_UNDIRECTED;
	t_node	**tmpList = ft_calloc(max_vertex_count, sizeof(t_node*));
	if (!tmpList)
		return 0;
	for (int i = 0; i < max_vertex_count; i++)
	{
		tmpList[i] = ft_calloc(1, sizeof(t_node));
		if (!tmpList[i])
		{
			while (--i > -1)
				free(tmpList[i]);
			free(tmpList);
			return 0;
		}
		tmpList[i]->vertex_id = i;
	}
	tmp->edge = tmpList;
	int	*tmpVertex = ft_calloc(max_vertex_count, sizeof(int));
	if (!tmpVertex)
		return 0;
	tmp->vertex = tmpVertex;
	return tmp;
}

void display_graph(t_graph* graph)
{
	if (!graph->current_vertex_count)
	{
        printf("Graph is Empty.\n");
        return ;
    }
	printf("\n< Graph > - ");
	//if (graph->graphType == GRAPH_DIRECTED)
		printf("directed\n");
	//else
	//	printf("undirected\n");
	/* edge print */
	for (int i = 0; i < graph->max_vertex_count; i++)
    {
        t_node *tmp = graph->edge[i];
		if (is_vertex_valid(graph, tmp->vertex_id))
			printf("\e[32m(O)");
		else	printf("\e[31m(X)");
        while (tmp)
        {
            printf("%d(%2f,%2f)\e[0m", tmp->vertex_id, tmp->screen_x, tmp->screen_y);
            tmp = tmp->next;
			if (tmp) printf(" -> ");
        }
        printf("\n");
    }
	/* vertex print */
	printf("Vertex: ");
	for (int i = 0; i < graph->max_vertex_count; i++)
	{
		if (graph->vertex[i])
			printf("\e[32m");
		else	printf("\e[31m");
		printf("%d\e[0m\t", graph->vertex[i]);
	}
	printf("\n\n");
}


int is_empty_graph(t_graph* graph)
{
	if (graph->current_vertex_count)
		return FALSE;
	return TRUE;
}

// 노드 추가
int add_vertex(t_graph* graph, int vertex_id)
{
	if (vertex_id < 0 || vertex_id >= graph->max_vertex_count)
		return FAIL;
	if (!graph->vertex[vertex_id])
   		graph->current_vertex_count++;
   	graph->vertex[vertex_id] = USED;
	return SUCCESS;
}

// 간선 추가
int add_edge(t_graph* graph, int from_vertex_id, int to_vertex_id)
{
    if (!is_vertex_valid(graph, from_vertex_id) || !is_vertex_valid(graph, to_vertex_id))
		return FAIL;
    // if ((from_vertex_id < 0 || from_vertex_id >= graph->max_vertex_count) || (to_vertex_id < 0 || to_vertex_id >= graph->max_vertex_count))
	// 	return FAIL;
	if (from_vertex_id == to_vertex_id)
		return FAIL;
    //if (findGraphNodePosition(graph->edge[from_vertex_id], to_vertex_id))
    //    return (FAIL);

	t_node  *tmp = ft_calloc(1, sizeof(t_node));
	if (!tmp)	return FAIL;
	tmp->vertex_id = to_vertex_id;
	t_node	*ptr = graph->edge[from_vertex_id];
	while (ptr->next)
		ptr = ptr->next;
    ptr->next = tmp;
	/*if (graph->graphType == GRAPH_UNDIRECTED)
	//{
		tmp = calloc(1, sizeof(t_node));
		if (!tmp)	return FAIL;
		tmp->vertex_id = from_vertex_id;
		ptr = graph->edge[to_vertex_id];
		while (ptr->next)
			ptr = ptr->next;
    	ptr->next = tmp;
	//}*/
    graph->current_edge_count++;
	return SUCCESS;
}
