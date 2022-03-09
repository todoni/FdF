#include "graph.h"
#include "fdf.h"
#include "mlx.h"
#include "libft.h"
#include "error_messages.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	init_color(t_gradient *dt)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		dt->sum[i] = 0;
		i++;
	}
	dt->from[0] = (0xFF0000 & 0b111111110000000000000000) >> 16;
	dt->from[1] = (0xFF0000 & 0b000000001111111100000000) >> 8;
	dt->from[2] = 0xFF0000 & 0b000000000000000011111111;
	dt->to[0] = (0xFF0000 & 0b111111110000000000000000) >> 16;
	dt->to[1] = (0xFF0000 & 0b000000001111111100000000) >> 8;
	dt->to[2] = 0xFF0000 & 0b000000000000000011111111;
}

void	set_pixel_color(int *color, char *rgb)
{
	int	full_color;

	full_color = ft_htoi(rgb);
	color[0] = (full_color & 0b111111110000000000000000) >> 16;
	color[1] = (full_color & 0b000000001111111100000000) >> 8;
	color[2] = full_color & 0b000000000000000011111111;
}

void	init_bresenham_param(t_bresenham *bh, t_node *from, t_node *to)
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

void	set_bresenham_param(t_bresenham *bh)
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

void	get_delta_color(t_gradient *dt)
{
	dt->delta[0] = dt->to[0] - dt->from[0];
	dt->delta[1] = dt->to[1] - dt->from[1];
	dt->delta[2] = dt->to[2] - dt->from[2];
}

void	set_bresenham_error(t_bresenham *bh)
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

void	set_color_gradient(t_gradient *dt)
{
	dt->sum[0] += dt->delta[0];
	dt->sum[1] += dt->delta[1];
	dt->sum[2] += dt->delta[2];
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
		colors = ((dt->from[0] + (dt->sum[0] / (bh->distance)))<< 16) + ((dt->from[1] + (dt->sum[1] / (bh->distance))) << 8) + (dt->from[2] +  (dt->sum[2] / (bh->distance)));
		if (bh->x > MAX_SCREEN_WIDTH || bh->y > MAX_SCREEN_HEIGHT)
			continue;
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

int	find_x_max(t_coordinate *coor, int num_point)
{
	int	index;
	int	max;

	index = 0;
	max = -2147483648;
	while (index < num_point)
	{
		if (coor[index].screen_x > max)
			max = coor[index].screen_x;
		index++;
	}
	return (max);
}

int	find_y_min(t_coordinate *coor, int num_point)
{
	int	index;
	int	min;

	index = 0;
	min = 2147483647;
	while (index < num_point)
	{
		if (coor[index].screen_y < min)
			min = coor[index].screen_y;
		index++;
	}
	return (min);
}

int	find_x_min(t_coordinate *coor, int num_point)
{
	int	index;
	int	min;

	index = 0;
	min = 2147483647;
	while (index < num_point)
	{
		if (coor[index].screen_x < min)
			min = coor[index].screen_x;
		index++;
	}
	return (min);
}

int	find_y_max(t_coordinate *coor, int num_point)
{
	int	index;
	int	max;

	index = 0;
	max = -2147483648;
	while (index < num_point)
	{
		if (coor[index].screen_y > max)
			max = coor[index].screen_y;
		index++;
	}
	return (max);
}

void	set_screen_coordinate(t_map *map)
{
	int		index;

	index = 0;
	while (index < map->size)
	{
		map->coor[index].x *= map->scale;
		map->coor[index].y *= map->scale;
		map->coor[index].z *= map->scale;
		map->coor[index].screen_x = (cos(map->rotation_y) * map->coor[index].x - sin(map->rotation_y) * map->coor[index].z);
		map->coor[index].screen_y = -(sin(map->rotation_x) * sin(map->rotation_y) * map->coor[index].x \
										  + cos(map->rotation_x) * map->coor[index].y \
							   + (sin(map->rotation_x) * cos(map->rotation_y) * map->coor[index].z));
		index++;
	}
}

int	key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
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

void	set_offset(t_map *map)
{
	int	index;
	int	offset;

	index = 0;
	offset = ft_abs(find_y_min(map->coor, map->size));
	while (index < map->size)
	{
		map->coor[index].screen_y += offset + 5;
		map->coor[index].screen_x += 5;
		index++;
	}
}

void	connect_vertexes(t_graph *undirect, t_map map, t_coordinate *coor)
{
	int i;

	i = 0;
	while (i < map.size)
	{
		add_vertex(undirect, i, coor);
		i++;
	}
	i = 0;
	while (i < map.size)
	{
		add_edge(undirect, i, i + 1, coor);
		i++;
		if ((i + 1) % map.column == 0)
			i++;
	}
	i = 0;
	while (i + map.column < map.size)
	{
		add_edge(undirect, i , i + map.column, coor);
		i++;
	}
	i = 0;
	while (i + map.column < map.size)
	{
		add_edge(undirect, i + map.column, i + 1, coor);
		i++;
		if ((i + 1) % map.column == 0)
			i++;
	}
}

void	init_map(t_map *map)
{
	map->column = 0;
	map->row = 0;
	map->size = 0;
	map->screen_width = 0;
	map->screen_height = 0;
	map->scale = 0;
	map->rotation_x = asin(tan(26.565 * M_PI / 180));
	map->rotation_y = -45 * M_PI / 180;
	map->coor = 0;
}

void	get_screen_size(t_map *map)
{
	map->screen_width = ft_abs(find_x_max(map->coor, map->size) - find_x_min(map->coor, map->size));
	map->screen_height = ft_abs(find_y_min(map->coor, map->size) - find_y_max(map->coor, map->size));
}

void	scale_map(t_map *map)
{
	if (map->screen_height > MAX_SCREEN_HEIGHT)
	{
		//map->scale = MAX_SCREEN_HEIGHT / (double)map->screen_height;
		//resize_y(map->coor, map->size, map->scale);
		map->screen_height = MAX_SCREEN_HEIGHT;
	}
	if (map->screen_width > MAX_SCREEN_WIDTH)
	{
		//map->scale = MAX_SCREEN_WIDTH / (double)map->screen_width;
		//resize_x(map->coor, map->size, map->scale);
		map->screen_width = MAX_SCREEN_WIDTH;
	}
}

void	set_mlx(t_data *data, t_map *map)
{
	data->mlx = mlx_init();
	data->img = mlx_new_image(data->mlx, map->screen_width + 10, map->screen_height + 10);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel, &data->line_length, &data->endian);
	data->mlx_win = mlx_new_window(data->mlx, map->screen_width + 10, map->screen_height + 10, "fdf");
}

void	set_coordinate_visibility(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->size)
	{
		if (map->coor[i].screen_x > MAX_SCREEN_WIDTH)
			map->coor[i].visible = NOT_VISIBLE;
		if (map->coor[i].screen_y > MAX_SCREEN_HEIGHT)
			map->coor[i].visible = NOT_VISIBLE;
		i++;
	}
}

void	set_map(t_map *map, t_file *file)
{
	map->block = read_map(file->fd, map);
	get_map_size(map->block, map);
	close(file->fd);
	map->coor = ft_calloc(map->size, sizeof(t_coordinate));
	if (!map->coor)
		terminate(ERR_MEMORY_ALLOCATION);
	make_coordinate(map->block, map->coor);
	get_map_color(map->block, map->coor);
	set_screen_coordinate(map);
	set_offset(map);
	get_screen_size(map);
	scale_map(map);
}

int main(int argc, char **argv)
{
	t_graph				*undirect;
	t_file				file;
	t_data				data;
	t_map				map;

	if (argc != 3)
		terminate(ERR_ARGUMENTS);
	file.name = argv[1];
	file.fd = open(file.name, O_RDONLY);
	if (file.fd == -1)
		terminate(file.name);
	init_map(&map);
	map.scale = ft_atoi(argv[2]);
	if (map.scale == 0)
		map.scale = ZOOM_DEFAULT;
	set_map(&map, &file);
	set_mlx(&data, &map);
	undirect = create_graph(map.size);
	connect_vertexes(undirect, map, map.coor);
	draw_dfs(undirect, 0, &data);
	mlx_put_image_to_window(data.mlx, data.mlx_win, data.img, 0, 0);
	deleteLinkedGraph(undirect);
	mlx_hook(data.mlx_win, X_EVENT_KEY_PRESS, 0, &key_press, 0);
	mlx_loop(data.mlx);
	return (0);
}
