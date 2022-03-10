#include "../includes/fdf.h"

static void	init_bresenham_param(t_bresenham *bh, t_node *from, t_node *to)
{
	bh->x = *(from->screen_x);
	bh->y = *(from->screen_y);
	bh->x_next = *(to->screen_x);
	bh->y_next = *(to->screen_y);
	bh->delta_x = ft_abs(bh->x_next - bh->x);
	bh->delta_y = ft_abs(bh->y_next - bh->y);
	bh->distance = sqrt(bh->delta_x * bh->delta_x + bh->delta_y * bh->delta_y);
	if (bh->distance == 0)
		bh->distance = 1;
}

static void	set_bresenham_param(t_bresenham *bh)
{
	bh->add_x = 1;
	bh->add_y = 1;
	bh->err = bh->delta_x / 2;
	if (bh->x >= bh->x_next)
		bh->add_x = -1;
	if (bh->y >= bh->y_next)
		bh->add_y = -1;
	if (bh->delta_x <= bh->delta_y)
		bh->err = -(bh->delta_y / 2);
}

static void	set_bresenham_error(t_bresenham *bh)
{
	bh->e2 = bh->err;
	if (bh->e2 > -bh->delta_x)
	{
		bh->err -= bh->delta_y;
		bh->x += bh->add_x;
	}
	if (bh->e2 < bh->delta_y)
	{
		bh->err += bh->delta_x;
		bh->y += bh->add_y;
	}
}

static void	draw_bresenham_line(t_gradient *dt, t_bresenham *bh, t_data *data)
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

void	draw_map(t_graph *graph, int vertex_id, t_data *data)
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
		draw_bresenham_line(&dt, &param, data);
		draw_map(graph, closeNode->vertex_id, data);
		closeNode = closeNode->next;
	}
}
