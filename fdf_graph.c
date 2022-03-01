#include "fdf_graph.h"
#include <stdio.h>

int is_vertex_valid(t_graph *graph, int vertex_id)
{
	if (graph->vertex[vertex_id])
        return (TRUE);
    return (FALSE);
}

void deleteLinkedGraph(t_graph* pGraph)
{
	if (!pGraph->current_vertex_count)
		return ;
	/* ppAdjEdge free */
	for (int i = 0; i < pGraph->max_vertex_count; i++)
	{
		while (pGraph->edge[i])
		{
		    t_node 	*tmp = pGraph->edge[i];
            pGraph->edge[i] = pGraph->edge[i]->next;
			//free(tmp->color);
			free(tmp);
		}
	}
	free(pGraph->edge);
	pGraph->edge = NULL;
	free(pGraph->vertex);
    pGraph->vertex = NULL;
	free(pGraph);
	//pGraph->max_vertex_count = pGraph->current_vertex_count = pGraph->current_edge_count = 0;
	printf("Graph deleted\n");
}


t_node	**create_list(int max_vertex_count)
{
	t_node	**tmp_list; 
	int		index;

	index = 0;
	tmp_list = ft_calloc(max_vertex_count, sizeof(t_node*));
	if (!tmp_list)
		return 0;
	while (index < max_vertex_count)
	{
		tmp_list[index] = ft_calloc(1, sizeof(t_node));
		if (!tmp_list[index])
		{
			while (--index > -1)
				free(tmp_list[index]);
			free(tmp_list);
			return (0);
		}
		tmp_list[index]->vertex_id = index;
		index++;
	}
	return (tmp_list);
}

t_graph* create_graph(int max_vertex_count)
{
    t_graph	*tmp;
	t_node	**tmp_list;
	int	*tmp_vertex;

	tmp = ft_calloc(1, sizeof(t_graph));
	if (!tmp)
	    return 0;
	tmp->max_vertex_count = max_vertex_count;
	tmp->current_vertex_count = 0;
	tmp_list = create_list(max_vertex_count);
	tmp->edge = tmp_list;
	tmp_vertex = ft_calloc(max_vertex_count, sizeof(int));
	if (!tmp_vertex)
		return 0;
	tmp->vertex = tmp_vertex;
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
        while (tmp && tmp->screen_x)
        {
            //printf("%d(%2f,%2f)\e[0m", tmp->vertex_id, *(tmp->screen_x), *(tmp->screen_y));
            //printf("%d(%2f,%2f)\e[0m", tmp->vertex_id, *(tmp->screen_x), *(tmp->screen_y));
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
int add_vertex(t_graph* graph, int vertex_id, t_coordinate *coor)
{
	if (vertex_id < 0 || vertex_id >= graph->max_vertex_count)
		return FAIL;
	if (!graph->vertex[vertex_id])
   		graph->current_vertex_count++;
   	graph->vertex[vertex_id] = USED;
	t_node *ptr;
	ptr = graph->edge[vertex_id];
	ptr->screen_x = &coor[vertex_id].screen_x;
	ptr->screen_y = &coor[vertex_id].screen_y;
	ptr->color = coor[vertex_id].color;
	return SUCCESS;
}

// 간선 추가
int add_edge(t_graph* graph, int from_vertex_id, int to_vertex_id, t_coordinate *coor)
{
    if (!is_vertex_valid(graph, from_vertex_id) || !is_vertex_valid(graph, to_vertex_id))
		return FAIL;
    if ((from_vertex_id < 0 || from_vertex_id >= graph->max_vertex_count) || (to_vertex_id < 0 || to_vertex_id >= graph->max_vertex_count))
	 	return FAIL;
	if (from_vertex_id == to_vertex_id)
		return FAIL;
    //if (findGraphNodePosition(graph->edge[from_vertex_id], to_vertex_id))
    //    return (FAIL);

	t_node  *tmp; 
	tmp = ft_calloc(1, sizeof(t_node));
	if (!tmp)
		return FAIL;
	tmp->vertex_id = to_vertex_id;
	tmp->screen_x = &coor[to_vertex_id].screen_x;
	tmp->screen_y = &coor[to_vertex_id].screen_y;
	//tmp->visible = &coor[to_vertex_id].visible;
	tmp->color = coor[to_vertex_id].color;
	t_node	*ptr;
	ptr = graph->edge[from_vertex_id];
	//ptr->screen_x = &coor[from_vertex_id].screen_x;
	//ptr->screen_y = &coor[from_vertex_id].screen_y;
	while (ptr->next)
		ptr = ptr->next;
    ptr->next = tmp;
    graph->current_edge_count++;
	return SUCCESS;
}
