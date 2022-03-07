#include "fdf_graph.h"
#include "minilibx_mms_20210621//mlx.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>



#define VISITED 1


typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_matrix
{
	double	alpha;
	double	beta;
	double	cos_alpha;
	double	cos_beta;
	double	sin_alpha;
	double	sin_beta;
}				t_matrix;


void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

typedef struct	s_offset
{
	double	y;
	double	x;
}				t_offset;

typedef	struct	s_colors
{
	int	from_r;
	int	from_g;
	int	from_b;
	int	to_r;
	int	to_g;
	int	to_b;
	int	delta_r;
	int	delta_g;
	int	delta_b;
}				t_color;


void	traversal_DFS_recursion2(t_graph *graph, int vertex_id, t_data *data, t_offset offset)
{
	t_node	*closeNode;
	int		x;
	int		y;
	int		pixels;
	int		x_next;
	int		y_next;
	int		delta_x;
	int		delta_y;
	int		add_x;
	int		add_y;
	int		err;
	int		e2;
	t_color	color;
	int		full_color_to;
	int		full_color_from;
	int		colors;
	int		sum_r;
	int		sum_g;
	int		sum_b;

	//printf("%d\n", vertex_id);
	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	graph->edge[vertex_id]->visited = VISITED;
	closeNode = graph->edge[vertex_id]->next;
	color.from_r = 0xFF0000 & 0b111111110000000000000000;
	color.from_g = 0xFF0000 & 0b000000001111111100000000;
	color.from_b = 0xFF0000 & 0b000000000000000011111111;
	color.to_r = 0xFF0000 & 0b111111110000000000000000;
	color.to_g = 0xFF0000 & 0b000000001111111100000000;
	color.to_b = 0xFF0000 & 0b000000000000000011111111;
	color.from_r >>= 16;
	color.from_g >>= 8;
	color.to_r >>= 16;
	color.to_g >>= 8;
	while (closeNode)
	{
		sum_r = 0;
		sum_b = 0;
		sum_g = 0;
		x = *graph->edge[vertex_id]->screen_x;
		y = *graph->edge[vertex_id]->screen_y;
		x_next = *(closeNode->screen_x);
		y_next = *(closeNode->screen_y);
		delta_x = abs(x_next - x);
		delta_y = abs(y_next - y);
		pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		if (pixels == 0)
			pixels = 1;
		if (closeNode->color)
		{
			full_color_to = strtol(closeNode->color, NULL, 16);
			color.to_r = (full_color_to & 0b111111110000000000000000) >> 16;
			color.to_g = (full_color_to & 0b000000001111111100000000) >> 8;
			color.to_b = full_color_to & 0b000000000000000011111111;
		}
		if (graph->edge[vertex_id]->color)
		{	
			full_color_from = strtol(graph->edge[vertex_id]->color, NULL, 16);
			color.from_r = (full_color_from & 0b111111110000000000000000) >> 16;
			color.from_g = (full_color_from & 0b000000001111111100000000) >> 8;
			color.from_b = full_color_from & 0b000000000000000011111111;
		}
		color.delta_r = color.to_r - color.from_r;
		color.delta_g = color.to_g - color.from_g;
		color.delta_b = color.to_b - color.from_b;
 
		add_x = 1;
		add_y = 1; 
		err = delta_x / 2; 
		if (x >= x_next)
			add_x = -1;
		if (y >= y_next)
			add_y = -1;
		if (delta_x <= delta_y)
			err = -delta_y / 2;
		while (1)
		{
			colors = ((color.from_r + (sum_r / (pixels)))<< 16) + ((color.from_g + (sum_g / (pixels))) << 8) + (color.from_b +  (sum_b/ (pixels)));
			my_mlx_pixel_put(data, x, offset.y + y, colors);
    		if (x == x_next && y == y_next)
				break ;
   	 		e2 = err;
    		if (e2 > -delta_x)
			{
				err -= delta_y;
				x += add_x;
			}
    		if (e2 < delta_y)
			{
				err += delta_x;
				y += add_y;
			}
			sum_r += color.delta_r;
			sum_g += color.delta_g;
			sum_b += color.delta_b;
		}	
		traversal_DFS_recursion2(graph, closeNode->vertex_id, data, offset);
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

#define X_EVENT_KEY_PRESS		2

#define KEY_ESC			53
#define MAX_SCREEN_WIDTH 1440
#define MAX_SCREEN_HEIGHT 900
#define ZOOM_DEFAULT 20

void	matrix_calc(t_coordinate *coor, t_matrix component, t_map map, int distance)
{
	int		index;

	index = 0;
	while (index < map.size)
	{
		coor[index].x *= ZOOM_DEFAULT;
		coor[index].y *= ZOOM_DEFAULT;
		coor[index].z *= ZOOM_DEFAULT;
		coor[index].screen_x = (cos(component.beta) * coor[index].x - sin(component.beta) * coor[index].z);
		coor[index].screen_y = -(sin(component.alpha) * sin(component.beta) * coor[index].x \
										  + cos(component.alpha) * coor[index].y \
							   + (sin(component.alpha) * cos(component.beta) * coor[index].z));
		index++;
	}
}

int	key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

void	resize(t_coordinate *coor, int size, double z_scale)
{
	int	index;
	
	index = 0;
	while (index < size)
	{
		coor[index].screen_y *= z_scale;
		index++;
	}
}

void	resize2(t_coordinate *coor, int size, double z_scale)
{
	int	index;
	
	index = 0;
	while (index < size)
	{
		coor[index].screen_x *= z_scale;
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

#include <errno.h>
#include "error_messages.h"

int main(int argc, char **argv)
{
	t_graph				*undirect;
	int					fd;
	t_coordinate		*coor;
	void				*mlx;
	void				*mlx_win;
	t_data				img;
	int					screen_width;
	int					screen_height;
	t_matrix			component;
	t_offset			offset;
	char				*filename;
	double				z_scale;
	double				zoom;
	t_map				map;
	t_list				*map_block;

	map.size = 0;
	map.column = 0;
	map.row = 0;
	if (argc != 2)
		terminate(ERR_ARGUMENTS);
	filename = argv[1];
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		terminate(filename);
	map_block = read_map(fd, &map);
	get_map_size(map_block, &map);
	close(fd);
	coor = ft_calloc(map.size, sizeof(t_coordinate));
	if (!coor)
		terminate(ERR_MEMORY_ALLOCATION);
	make_coordinate(map_block, coor);
	get_map_color(map_block, coor);
	component.alpha = asin(tan(26.565 * M_PI / 180));
	component.beta = -45 * M_PI / 180;

	mlx = mlx_init();
	screen_width = 0;
	screen_height = 0;
	zoom = ZOOM_DEFAULT;
	undirect = create_graph(map.size);
	connect_vertexes(undirect, map, coor);
	matrix_calc(coor, component, map, 20);
	screen_width = abs(find_x_max(coor, map.size) - find_x_min(coor, map.size));
	screen_height = abs(find_y_min(coor, map.size) - find_y_max(coor, map.size));
	z_scale = 0;
	if (screen_height >= MAX_SCREEN_HEIGHT)
	{
		z_scale = MAX_SCREEN_HEIGHT / (double)screen_height;
		resize(coor, map.size, z_scale);
		screen_height = MAX_SCREEN_HEIGHT;
	}
	if (screen_width >= MAX_SCREEN_WIDTH)
	{
		z_scale = MAX_SCREEN_WIDTH / (double)screen_width;
		resize2(coor, map.size, z_scale);
		screen_width = MAX_SCREEN_WIDTH;
	}
	offset.x = abs(find_x_min(coor, map.size));
	offset.y = abs(find_y_min(coor, map.size));
	printf("%d %d\n", screen_width, screen_height);
	img.img = mlx_new_image(mlx, screen_width + 10, screen_height + 10);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	traversal_DFS_recursion2(undirect, 0, &img, offset);
	if (screen_width >= MAX_SCREEN_WIDTH)
		screen_width = MAX_SCREEN_WIDTH;
	if (screen_height >= MAX_SCREEN_HEIGHT)
		screen_height = MAX_SCREEN_HEIGHT;
	mlx_win = mlx_new_window(mlx, screen_width + 10, screen_height + 10, "fdf");
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	deleteLinkedGraph(undirect);
	mlx_hook(mlx_win, X_EVENT_KEY_PRESS, 0, &key_press, 0);
	mlx_loop(mlx);
}
