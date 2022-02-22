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
	int	y;
	int	x;
}				t_offset;

void	traversal_DFS_recursion2(t_graph *graph, int vertex_id, t_data *data, t_offset offset)
{
	t_node	*closeNode;
	int		scale;
	double	x;
	double	y;
	double	pixels;
	double	x_next;
	double	y_next;
	double	delta_x;
	double	delta_y;

	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	graph->edge[vertex_id]->visited = VISITED;
	closeNode = graph->edge[vertex_id]->next;
	while (closeNode)
	{
		x = *graph->edge[vertex_id]->screen_x;
		y = *graph->edge[vertex_id]->screen_y;
		x_next = *(closeNode->screen_x);
		y_next = *(closeNode->screen_y);
		delta_x = (x_next - x);
		delta_y = (y_next - y);
		pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		
		delta_x /= pixels;
		delta_y /= pixels;
		while (round(pixels))
		{
			//if (closeNode->color)
			//{
			//	my_mlx_pixel_put(data, round(x), offset.y + round(y), strtol(closeNode->color, NULL, 16));
				//free(closeNode->color);
			//}
			//else
			my_mlx_pixel_put(data, 720 + round(x), 600 + round(y), 0xFF0000);
    		x += delta_x;
    		y += delta_y;
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
#define MAX_SCREEN_WIDTH 1680
#define MAX_SCREEN_HEIGHT 1050

void	matrix_calc(t_coordinate *coor, t_matrix component, int	size, int distance)
{
	int		index;
	double	f = 1000;
	double	n = 0.1;

	index = 0;
	while (index < size)
	{
		//coor[index].x *= ZOOM_DEFAULT;
		coor[index].y *= -1;
		coor[index].screen_x = (cos(component.beta) * coor[index].x + sin(component.beta) * coor[index].z);
		coor[index].screen_y = (sin(component.alpha) * sin(component.beta) * coor[index].x \
										  + cos(component.alpha) * coor[index].y \
							   - (sin(component.alpha) * cos(component.beta) * coor[index].z));
		coor[index].new_z = (-(cos(component.alpha) * sin(component.beta) * coor[index].x) \
										  + sin(component.alpha) * coor[index].y \
							   + cos(component.alpha) * cos(component.beta) * coor[index].z);
		//coor[index].new_z = - (f / (f - n)) * coor[index].new_z - 1;

		//coor[index].new_z += distance;
		//printf("%d: %f %f %f\n",index, coor[index].screen_x, coor[index].screen_y, coor[index].new_z);
		//printf("%d: %f %f %f\n",index, coor[index].screen_x, coor[index].screen_y, coor[index].new_z);
		coor[index].new_z += distance;
		coor[index].screen_x /= coor[index].new_z;
		coor[index].screen_y /= coor[index].new_z;
		coor[index].screen_x *= 1440;
		coor[index].screen_y *= 900;
		coor[index].screen_x /= 8;
		coor[index].screen_y /= 6; // ??
		//coor[index].z *= ZOOM_DEFAULT;
		printf("%d: %f %f %f\n",index, coor[index].screen_x, coor[index].screen_y, coor[index].new_z);

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
	clock_t	start, end, sum;
	t_map				map;

	filename = argv[1];
	fd = open(filename, O_RDONLY);
	//read(fd, filename, 1);
	if (fd == -1)
	{
		perror(filename);
		//write(2, filename, ft_strlen(filename));
		//printf("%s\n", strerror(5));
		//write(2, strerror(3), ft_strlen(strerror(3)));
		exit(1);

	}
	int	fd2 = open(filename, O_RDONLY);
	save = 0;
	coor = (t_coordinate *)ft_calloc(10000000, sizeof(t_coordinate));
	if (!coor)
		return (1);
	//coor2 = (t_coordinate *)ft_calloc(10000000, sizeof(t_coordinate));
	start = clock();
	//read_map(fd, &save, &map);
	if (read_map2(fd, coor, &map) == -1)
		return (1);
	//int	point_per_line2 = read_map2(fd2, coor2, &map);
	//printf("22 %d\n", point_per_line2);
	end = clock();
	sum += (end - start);
	printf("read_map :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	close(fd);
	start = clock();
	//num_point = count_size(save);
	num_point = map.size;
	point_per_line = map.column;
	//int	num_point2 = map.size;
	printf("%d %d \n", point_per_line, num_point);
	end = clock();
	sum += (end - start);
	printf("count list size :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	//coor = (t_coordinate *)ft_calloc(num_point, sizeof(t_coordinate));
	end = clock();
	sum += (end - start);
	printf("calloc coor :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	//make_coor(coor, save, num_point);
	end = clock();
	sum += (end - start);
	printf("give value to coordinate :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	printf("time elasped: %f\n", (float)sum / CLOCKS_PER_SEC);
	component.alpha = asin(tan(30 * M_PI / 180));
	component.beta = 45 * M_PI / 180;

	mlx = mlx_init();
	start = clock();
	undirect = create_graph(num_point);
	//undirect2 = create_graph(num_point);
	end = clock();
	printf("create graph :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	connect_vertexes(undirect, map, coor);
	//connect_vertexes(undirect2, point_per_line, num_point, coor2);
	end = clock();
	printf("add edges :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	matrix_calc(coor, component, num_point, 20);
	//matrix_calc(coor2, component, num_point, 120);
	end = clock();
	printf("translate coordinate :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	//display_graph(undirect);
	start = clock();
	screen_width = find_x_max(coor, num_point);
	end = clock();
	printf("find screen width :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	screen_height = abs(find_y_min(coor, num_point)) + abs(find_y_max(coor, num_point));
	end = clock();
	printf("find screen height :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	z_scale = 0;
	if (screen_height >= MAX_SCREEN_HEIGHT)
	{
		printf("%d\n", screen_height);
		z_scale = MAX_SCREEN_HEIGHT / (double)screen_height;
		printf("%f\n", z_scale);
		start = clock();
		resize(coor, num_point, z_scale);
		end = clock();
		printf("resize y :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	}
	if (screen_width >= MAX_SCREEN_WIDTH)
	{
		z_scale = MAX_SCREEN_WIDTH / (double)screen_width;
		start = clock();
		resize2(coor, num_point, z_scale);
		end = clock();
		printf("resize x :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	}
	offset.x = abs(find_x_max(coor, num_point));
	offset.y = abs(find_y_min(coor, num_point));
	img.img = mlx_new_image(mlx, 3000,3000);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	start = clock();
	traversal_DFS_recursion2(undirect, 0, &img, offset);
	//traversal_DFS_recursion2(undirect2, 0, &img, offset);
	end = clock();
	printf("traversal :%f\n", (float)(end - start) / CLOCKS_PER_SEC);
	if (screen_width >= MAX_SCREEN_WIDTH)
		screen_width = MAX_SCREEN_WIDTH;
	if (screen_height >= MAX_SCREEN_HEIGHT)
		screen_height = MAX_SCREEN_HEIGHT;
	mlx_win = mlx_new_window(mlx, 3000, 3000, "fdf");
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	deleteLinkedGraph(undirect);
	mlx_hook(mlx_win, X_EVENT_KEY_PRESS, 0, &key_press, 0);
	mlx_loop(mlx);
}
