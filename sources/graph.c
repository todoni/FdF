#include "../includes/fdf.h"

t_node	**create_list(int max_vertex_count)
{
	t_node	**tmp_list;
	int		index;

	index = 0;
	tmp_list = ft_calloc(max_vertex_count, sizeof(t_node *));
	if (!tmp_list)
		return (FAIL);
	while (index < max_vertex_count)
	{
		tmp_list[index] = ft_calloc(1, sizeof(t_node));
		if (!tmp_list[index])
		{
			while (--index > -1)
				free(tmp_list[index]);
			free(tmp_list);
			return (FAIL);
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
		return (FAIL);
	tmp->max_vertex_count = max_vertex_count;
	tmp->current_vertex_count = 0;
	tmp_list = create_list(max_vertex_count);
	if (!tmp_list)
		return (FAIL);
	tmp->edge = tmp_list;
	tmp_vertex = ft_calloc(max_vertex_count, sizeof(int));
	if (!tmp_vertex)
		return (FAIL);
	tmp->vertex = tmp_vertex;
	return (tmp);
}

int	add_vertex(t_graph *graph, int vertex_id, t_coordinate *coor)
{
	t_node	*ptr;

	if (vertex_id < 0 || vertex_id >= graph->max_vertex_count)
		return (FAIL);
	if (!graph->vertex[vertex_id])
		graph->current_vertex_count++;
	graph->vertex[vertex_id] = (USED);
	ptr = graph->edge[vertex_id];
	ptr->screen_x = &coor[vertex_id].screen_x;
	ptr->screen_y = &coor[vertex_id].screen_y;
	ptr->color = coor[vertex_id].color;
	return (SUCCESS);
}

int	add_edge(t_graph *graph, int from_vertex_id, \
									int to_vertex_id, t_coordinate *coor)
{
	t_node	*tmp;
	t_node	*ptr;

	tmp = ft_calloc(1, sizeof(t_node));
	if (!tmp)
		return (FAIL);
	tmp->vertex_id = to_vertex_id;
	tmp->screen_x = &coor[to_vertex_id].screen_x;
	tmp->screen_y = &coor[to_vertex_id].screen_y;
	tmp->color = coor[to_vertex_id].color;
	ptr = graph->edge[from_vertex_id];
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = tmp;
	graph->current_edge_count++;
	return (SUCCESS);
}
