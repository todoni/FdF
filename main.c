#include "fdf_graph.h"
#include "libft/libft.h"
#include "mlx.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>



#define VISITED 1
#define ZOOM_DEFAULT 20


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
	int		scale;
	double	x;
	double	y;
	int		pixels;
	double	x_next;
	double	y_next;
	double	delta_x;
	double	delta_y;
	t_color	color;
	int		full_color_to;
	int		full_color_from;
	int		colors;

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
	colors = 0;
	while (closeNode)
	{
		x = *graph->edge[vertex_id]->screen_x;
		y = *graph->edge[vertex_id]->screen_y;
		x_next = *(closeNode->screen_x);
		y_next = *(closeNode->screen_y);
		delta_x = (x_next - x);
		delta_y = (y_next - y);
		pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		if (closeNode->color)
		{
			full_color_to = strtol(closeNode->color, NULL, 16);
			color.to_r = full_color_to & 0b111111110000000000000000;
			color.to_g = full_color_to & 0b000000001111111100000000;
			color.to_b = full_color_to & 0b000000000000000011111111;
		}
		if (graph->edge[vertex_id]->color)
		{	
			full_color_from = strtol(graph->edge[vertex_id]->color, NULL, 16);
			color.from_r = full_color_from & 0b111111110000000000000000;
			color.from_g = full_color_from & 0b000000001111111100000000;
			color.from_b = full_color_from & 0b000000000000000011111111;
		}
		color.delta_r = (color.to_r >> 16) - (color.from_r >> 16);
		color.delta_g = (color.to_g >> 8) - (color.from_g >> 8);
		color.delta_b = color.to_b - color.from_b;
		
		delta_x /= pixels;
		delta_y /= pixels;
		color.delta_r /= pixels;
		color.delta_g /= pixels;
		color.delta_b /= pixels;

		while (pixels)
		{
			colors = color.from_r + color.from_g + color.from_b;
			my_mlx_pixel_put(data, round(x), offset.y + round(y), colors);
    		x += delta_x;
    		y += delta_y;
			color.from_r += color.delta_r << 16;
			color.from_g += color.delta_g << 8;
			color.from_b += color.delta_b;
			pixels--;
		}
		traversal_DFS_recursion2(graph, closeNode->vertex_id, data, offset);
		closeNode = closeNode->next;
	}
}

int	count_size(t_coordinate_list *coor)
{
	int count;

	count = 0;
	while (coor)
	{
		coor = coor->next;
		count++;
	}
	return (count);
}

void	make_coor(t_coordinate *coor, t_coordinate_list *save, int num_point)
{
	int	index;
	int	index2;

	index = 0;
	index2 = 0;
	while (index2 < num_point)
	{
		coor[index].x = save->block[index].x;
		coor[index].y = save->block[index].y;
		coor[index].z = save->block[index].z;
		index++;
		index2++;
		if (index2 % BUFFER_SIZE == 0)
		{
			save = save->next;
			index = 0;
		}
	}
}

void	initialize_visit(t_graph *undirect)
{
	int	index;

	index = 0;
	while (index < undirect->max_vertex_count)
	{
		undirect->edge[index]->visited = 0;
		index++;
	}
}

double	find_x_max(t_coordinate *coor, int num_point)
{
	int	index;
	double	max;

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

double	find_y_min(t_coordinate *coor, int num_point)
{
	int	index;
	double	min;

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

double	find_x_min(t_coordinate *coor, int num_point)
{
	int	index;
	double	min;

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

double	find_y_max(t_coordinate *coor, int num_point)
{
	int	index;
	double	max;

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

void	matrix_calc(t_coordinate *coor, t_matrix component, int	size, int distance)
{
	int		index;

	index = 0;
	while (index < size)
	{
		coor[index].x *= ZOOM_DEFAULT;
		coor[index].y *= -ZOOM_DEFAULT;
		coor[index].z *= ZOOM_DEFAULT;
		coor[index].screen_x = (cos(component.beta) * coor[index].x + sin(component.beta) * coor[index].z);
		coor[index].screen_y = (sin(component.alpha) * sin(component.beta) * coor[index].x \
										  + cos(component.alpha) * coor[index].y \
							   - (sin(component.alpha) * cos(component.beta) * coor[index].z));
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
		if ((i + 1) % map.column == 0)
			i++;
		add_edge(undirect, i, i + 1, coor);
		i++;
	}
	i = 0;
	while (i + map.column < map.size)
	{
		add_edge(undirect, i, i + map.column, coor);
		i++;
	}
}

#include <errno.h>

int main(int argc, char **argv)
{
	t_graph				*undirect;
	t_graph				*undirect2;
	int					num_point;
	int					fd;
	int					fd2;
	int					point_per_line;
	t_coordinate		*coor;
	t_coordinate		*coor2;
	t_coordinate_list	*save;
	void				*mlx;
	void				*mlx_win;
	t_data				img;
	int					screen_width;
	int					screen_height;
	t_matrix			component;
	t_offset			offset;
	char				*filename;
	double				z_scale;
	clock_t	start, end, sum = 0;
	t_map				map;

	filename = argv[1];
	fd = open(filename, O_RDONLY);
	fd2 = open(filename, O_RDONLY);
	if (fd == -1 || fd2 == -1)
	{
		perror(filename);
		exit(1);

	}
	save = 0;
	if (read_map1(fd, &map) == -1)
	{
		perror("failed to read map.");
		exit(1);
	}
	close(fd);
	coor = ft_calloc(map.size, sizeof(t_coordinate));
	if (!coor)
	{
		perror("Memory allocation failed.");
		exit(1);
	}
	start = clock();
	if (read_map2(fd2, coor) == -1)
	{
		perror("failed to read map.");
		exit(1);
	}
	close(fd2);
	end = clock();
	sum += (end - start);
	printf("read_map :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	printf("time elasped: %f\n", (float)sum / CLOCKS_PER_SEC);
	component.alpha = asin(tan(30 * M_PI / 180));
	component.beta = 45 * M_PI / 180;

	mlx = mlx_init();
	start = clock();
	undirect = create_graph(map.size);
	end = clock();
	printf("create graph :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	connect_vertexes(undirect, map, coor);
	end = clock();
	printf("add edges :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	matrix_calc(coor, component, map.size, 20);
	end = clock();
	printf("translate coordinate :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	screen_width = find_x_max(coor, map.size);
	end = clock();
	printf("find screen width :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	screen_height = fabs(find_y_min(coor, map.size)) + fabs(find_y_max(coor, map.size));
	end = clock();
	printf("find screen height :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	z_scale = 0;
	if (screen_height >= MAX_SCREEN_HEIGHT)
	{
		z_scale = MAX_SCREEN_HEIGHT / (double)screen_height;
		start = clock();
		resize(coor, map.size, z_scale);
		end = clock();
		printf("resize y :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	}
	if (screen_width >= MAX_SCREEN_WIDTH)
	{
		z_scale = MAX_SCREEN_WIDTH / (double)screen_width;
		start = clock();
		resize2(coor, map.size, z_scale);
		end = clock();
		printf("resize x :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	}
	offset.x = fabs(find_x_max(coor, map.size));
	offset.y = fabs(find_y_min(coor, map.size));
	img.img = mlx_new_image(mlx, screen_width, screen_height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	start = clock();
	traversal_DFS_recursion2(undirect, 0, &img, offset);
	end = clock();
	printf("traversal :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	if (screen_width >= MAX_SCREEN_WIDTH)
		screen_width = MAX_SCREEN_WIDTH;
	if (screen_height >= MAX_SCREEN_HEIGHT)
		screen_height = MAX_SCREEN_HEIGHT;
	mlx_win = mlx_new_window(mlx, screen_width, screen_height, "fdf");
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	deleteLinkedGraph(undirect);
	mlx_hook(mlx_win, X_EVENT_KEY_PRESS, 0, &key_press, 0);
	mlx_loop(mlx);
}
