/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 15:34:43 by sohan             #+#    #+#             */
/*   Updated: 2022/03/11 15:34:45 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static int	connect_horizontal(t_graph *graph, t_map *map, t_coordinate *coor)
{
	int	i;
	int	add_edge_return;

	i = 0;
	while (i < map->size)
	{
		add_edge_return = add_edge(graph, i, i + 1, coor);
		if (add_edge_return == FAIL)
			return (FAIL);
		++i;
		if ((i + 1) % map->column == 0)
			++i;
	}
	return (SUCCESS);
}

static int	connect_vertical(t_graph *graph, t_map *map, t_coordinate *coor)
{
	int	i;
	int	add_edge_return;

	i = 0;
	while (i + map->column < map->size)
	{
		add_edge_return = add_edge(graph, i, i + map->column, coor);
		if (add_edge_return == FAIL)
			return (FAIL);
		++i;
	}
	return (SUCCESS);
}

static int	connect_diagonal(t_graph *graph, t_map *map, t_coordinate *coor)
{
	int	i;
	int	add_edge_return;

	i = 0;
	while (i + map->column < map->size)
	{
		add_edge_return = add_edge(graph, i + map->column, i + 1, coor);
		if (add_edge_return == FAIL)
			return (FAIL);
		++i;
		if ((i + 1) % map->column == 0)
			++i;
	}
	return (SUCCESS);
}

static int	create_vertex(t_graph *graph, t_map *map, t_coordinate *coor)
{
	int	i;
	int	add_vertex_return;

	i = 0;
	while (i < map->size)
	{
		add_vertex_return = add_vertex(graph, i, coor);
		if (add_vertex_return == FAIL)
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}

int	connect_vertex(t_graph *graph, t_map *map, t_coordinate *coor)
{
	int	ret;

	ret = create_vertex(graph, map, coor);
	if (ret == FAIL)
		return (FAIL);
	ret = connect_horizontal(graph, map, coor);
	if (ret == FAIL)
		return (FAIL);
	ret = connect_vertical(graph, map, coor);
	if (ret == FAIL)
		return (FAIL);
	ret = connect_diagonal(graph, map, coor);
	if (ret == FAIL)
		return (FAIL);
	return (SUCCESS);
}
