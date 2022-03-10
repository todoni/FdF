#include "../includes/fdf.h"

void	get_screen_size(t_map *map)
{
	map->screen_width = ft_abs(find_x_max(map->coor, map->size));
	map->screen_height = ft_abs(find_y_min(map->coor, map->size) - \
										find_y_max(map->coor, map->size));
}

void	connect_vertexes(t_graph *graph, t_map *map, t_coordinate *coor)
{
	int	i;

	i = 0;
	while (i < map->size)
	{
		add_vertex(graph, i, coor);
		i++;
	}
	i = 0;
	while (i < map->size)
	{
		add_edge(graph, i, i + 1, coor);
		i++;
		if ((i + 1) % map->column == 0)
			i++;
	}
	i = 0;
	while (i + map->column < map->size)
	{
		add_edge(graph, i, i + map->column, coor);
		i++;
	}
}

void	connect_diagonal(t_graph *graph, t_map *map, t_coordinate *coor)
{
	int	i;

	i = 0;
	while (i + map->column < map->size)
	{
		add_edge(graph, i + map->column, i + 1, coor);
		i++;
		if ((i + 1) % map->column == 0)
			i++;
	}
}

void	resize_y(t_coordinate *coor, int size, double scale)
{
	int	index;

	index = 0;
	while (index < size)
	{
		coor[index].screen_y *= scale;
		index++;
	}
}

void	resize_x(t_coordinate *coor, int size, double scale)
{
	int	index;

	index = 0;
	while (index < size)
	{
		coor[index].screen_x *= scale;
		index++;
	}
}
