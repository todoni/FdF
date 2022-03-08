#include "graph.h"

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
	for (int i = 0; i < pGraph->max_vertex_count; i++)
	{
		while (pGraph->edge[i])
		{
		    t_node 	*tmp = pGraph->edge[i];
            pGraph->edge[i] = pGraph->edge[i]->next;
			free(tmp);
		}
	}
	free(pGraph->edge);
	pGraph->edge = NULL;
	free(pGraph->vertex);
    pGraph->vertex = NULL;
	free(pGraph);
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

t_graph	*create_graph(int max_vertex_count)
{
    t_graph	*tmp;
	t_node	**tmp_list;
	int		*tmp_vertex;

	tmp = ft_calloc(1, sizeof(t_graph));
	if (!tmp)
	    return (0);
	tmp->max_vertex_count = max_vertex_count;
	tmp->current_vertex_count = 0;
	tmp_list = create_list(max_vertex_count);
	if (!tmp_list)
		return (0);
	tmp->edge = tmp_list;
	tmp_vertex = ft_calloc(max_vertex_count, sizeof(int));
	if (!tmp_vertex)
		return (0);
	tmp->vertex = tmp_vertex;
	return (tmp);
}

int add_vertex(t_graph* graph, int vertex_id, t_coordinate *coor)
{
	if (vertex_id < 0 || vertex_id >= graph->max_vertex_count)
		return (FAIL);
	if (!graph->vertex[vertex_id])
   		graph->current_vertex_count++;
   	graph->vertex[vertex_id] = (USED);
	t_node *ptr;
	ptr = graph->edge[vertex_id];
	ptr->screen_x = &coor[vertex_id].screen_x;
	ptr->screen_y = &coor[vertex_id].screen_y;
	ptr->color = coor[vertex_id].color;
	return (SUCCESS);
}

int add_edge(t_graph* graph, int from_vertex_id, int to_vertex_id, t_coordinate *coor)
{
	t_node  *tmp; 

    if (!is_vertex_valid(graph, from_vertex_id) || !is_vertex_valid(graph, to_vertex_id))
		return (FAIL);
    if ((from_vertex_id < 0 || from_vertex_id >= graph->max_vertex_count) || (to_vertex_id < 0 || to_vertex_id >= graph->max_vertex_count))
	 	return (FAIL);
	if (from_vertex_id == to_vertex_id)
		return (FAIL);
	tmp = ft_calloc(1, sizeof(t_node));
	if (!tmp)
		return (FAIL);
	tmp->vertex_id = to_vertex_id;
	tmp->screen_x = &coor[to_vertex_id].screen_x;
	tmp->screen_y = &coor[to_vertex_id].screen_y;
	tmp->color = coor[to_vertex_id].color;
	t_node	*ptr;
	ptr = graph->edge[from_vertex_id];
	while (ptr->next)
		ptr = ptr->next;
    ptr->next = tmp;
    graph->current_edge_count++;
	return SUCCESS;
}
