#include "../includes/fdf.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	draw_line(t_gradient *dt, t_bresenham *bh, t_data *data)
{
	int	colors;

	while (1)
	{
		if (bh->x == bh->x_next && bh->y == bh->y_next)
			break ;
		set_bresenham_error(bh);
		set_color_gradient(dt);
		colors = ((dt->from[0] + (dt->sum[0] / (bh->distance))) << 16) + \
			((dt->from[1] + (dt->sum[1] / (bh->distance))) << 8) + \
			(dt->from[2] + (dt->sum[2] / (bh->distance)));
		if (bh->x > MAX_SCREEN_WIDTH || bh->y > MAX_SCREEN_HEIGHT)
			continue ;
		my_mlx_pixel_put(data, bh->x, bh->y, colors);
	}
}

void	draw_dfs(t_graph *graph, int vertex_id, t_data *data)
{
	t_node		*closeNode;
	t_node		*cur_node;
	t_bresenham	param;
	t_gradient	dt;

	cur_node = graph->edge[vertex_id];
	if (cur_node->visited == VISITED)
		return ;
	cur_node->visited = VISITED;
	closeNode = graph->edge[vertex_id]->next;
	while (closeNode)
	{
		init_color(&dt);
		if (closeNode->color)
			set_pixel_color(dt.to, closeNode->color);
		if (graph->edge[vertex_id]->color)
			set_pixel_color(dt.from, graph->edge[vertex_id]->color);
		get_delta_color(&dt);
		init_bresenham_param(&param, cur_node, closeNode);
		set_bresenham_param(&param);
		draw_line(&dt, &param, data);
		draw_dfs(graph, closeNode->vertex_id, data);
		closeNode = closeNode->next;
	}
}

void	scale_map(t_map *map)
{
	if (map->screen_height > MAX_SCREEN_HEIGHT)
	{
		map->scale = MAX_SCREEN_HEIGHT / (double)map->screen_height;
		resize_y(map->coor, map->size, map->scale);
		resize_x(map->coor, map->size, map->scale);
		map->screen_height = MAX_SCREEN_HEIGHT;
		map->screen_width = find_x_max(map->coor, map->size);
	}
	if (map->screen_width > MAX_SCREEN_WIDTH)
	{
		map->scale = MAX_SCREEN_WIDTH / (double)map->screen_width;
		resize_x(map->coor, map->size, map->scale);
		resize_y(map->coor, map->size, map->scale);
		map->screen_width = MAX_SCREEN_WIDTH;
		map->screen_height = ft_abs(find_y_max(map->coor, map->size) \
									- find_y_min(map->coor, map->size));
	}
}
