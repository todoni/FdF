#include "fdf_graph.h"
#include "libft/libft.h"
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
	int		scale;
	int		x;
	int		y;
	int		pixels;
	int		x_next;
	int		y_next;
	int		delta_x;
	int		delta_y;
	int		F;
	int		dxF1;
	int		dxF2;
	int		dyF1;
	int		dyF2;
	int		add_y;
	int		steps = 0;

	t_color	color;
	int		full_color_to;
	int		full_color_from;
	int		colors;
	int		sum_r;
	int		sum_g;
	int		sum_b;

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
	colors = 0xFF0000;
	while (closeNode)
	{
		colors = 0;
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
		F = 2 * delta_y - delta_x;
		dxF1 = 2 * delta_y;
		dxF2 = 2 * (delta_y - delta_x);
		dyF1 = 2 * delta_x;
		dyF2 = 2 * (delta_x - delta_y);

		if (closeNode->color)
		{
			full_color_to = strtol(closeNode->color, NULL, 16);
			color.to_r = full_color_to & 0b111111110000000000000000;
			color.to_g = full_color_to & 0b000000001111111100000000;
			color.to_b = full_color_to & 0b000000000000000011111111;
			color.to_r >>= 16;
			color.to_g >>= 8;
		}
		if (graph->edge[vertex_id]->color)
		{	
			full_color_from = strtol(graph->edge[vertex_id]->color, NULL, 16);
			color.from_r = full_color_from & 0b111111110000000000000000;
			color.from_g = full_color_from & 0b000000001111111100000000;
			color.from_b = full_color_from & 0b000000000000000011111111;
			color.from_r >>= 16;
			color.from_g >>= 8;
		}
		color.delta_r = color.to_r - color.from_r;
		color.delta_g = color.to_g - color.from_g;
		color.delta_b = color.to_b - color.from_b;
		
		//delta_x /= pixels;
		//delta_y /= pixels;
		//color.delta_r = (color.delta_r << 16) / pixels;
		//color.delta_g = (color.delta_g << 8) / pixels;
		//color.delta_b = color.delta_b  / pixels;
		//color.delta_g /= pixels;
		//color.delta_b /= pixels;
		
		
 
 		int dx = abs(x_next-x), sx = x<x_next ? 1 : -1;
  		int dy = abs(y_next-y), sy = y<y_next ? 1 : -1; 
		int err = (dx>dy ? dx : -dy)/2, e2;
		int	divisor = sqrt(dx * dx + dy * dy);
 
		for(;;){
				if (divisor == 0)
					divisor = 1;
				colors = ((color.from_r + (sum_r / (divisor)))<< 16) + ((color.from_g + (sum_g / (divisor))) << 8) + (color.from_b +  (sum_b/ (divisor)));
				
				my_mlx_pixel_put(data, offset.x + x, offset.y + y, colors);
				//printf("%d\n", colors);
    			if (x==x_next && y==y_next) break;
   	 			e2 = err;
    			if (e2 >-dx) { err -= dy; x += sx; }
    			if (e2 < dy) { err += dx; y += sy; }
				sum_r += color.delta_r;
				sum_g += color.delta_g;
				sum_b += color.delta_b;

		}		


		/*if (delta_x >= delta_y)
		{
			//color.delta_r /= delta_x;
			//color.delta_g /= delta_x;
			//color.delta_b /= delta_x;
			while (x <= x_next)
			{
				if (delta_x == 0 || delta_x == 1)
					delta_x = 2;
				colors = ((color.from_r + (sum_r / (delta_x - 1)))<< 16) + ((color.from_g + (sum_g / (delta_x - 1))) << 8) + (color.from_b +  (sum_b/ (delta_x - 1)));
				my_mlx_pixel_put(data, offset.x + x, offset.y + y, colors);
				//printf("color: %d\n", colors);
				//printf("F: %d\n", F);
				//printf("%d,%d -> %d,%d\n", x, y, x_next, y_next);
				if (F < 0)
					F += dxF1;
				else
				{
					if (y < y_next)
						y++;
					else
						y--;
					F += dxF2;
				}
				sum_r += color.delta_r;
				sum_g += color.delta_g;
				sum_b += color.delta_b;
				x++;
				//pixels--;
			}
		}
		else
		{
			//color.delta_r /= delta_y;
			//color.delta_g /= delta_y;
			//color.delta_b /= delta_y;
			if (y > y_next)
				add_y = -1;
			else
				add_y = 1;
			steps = 0;
			//printf("delta_y:%d\n", delta_y);
			while (steps < delta_y)
			{

				if (delta_y == 0 || delta_y == 1)
					delta_y = 2;
				//colors = ((color.from_r / (delta_y - 1)) << 16) + ((color.from_g / (delta_y - 1)) << 8) + (color.from_b / (delta_y - 1));
				colors = ((color.from_r + (sum_r / (delta_y - 1)))<< 16) + ((color.from_g + (sum_g / (delta_y - 1))) << 8) + (color.from_b +  (sum_b/ (delta_y - 1)));
				my_mlx_pixel_put(data, offset.x + x, offset.y + y, colors);
				//printf("F: %d\n", F);
				//printf("%d,%d -> %d,%d\n", x, y, x_next, y_next);
				if (F < 0)
					F += dyF1;
				else
				{
					if (x < x_next)
						x++;
					else
						x--;
					F += dyF2;
				}
				sum_r += color.delta_r;
				sum_g += color.delta_g;
				sum_b += color.delta_b;
				y += add_y;
				steps++;
				//pixels--;
			}
		}*/
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
	distance = 100;
	while (index < map.size)
	{
		//coor[index].x -= map.row;
		coor[index].x *= ZOOM_DEFAULT;
		coor[index].y *= ZOOM_DEFAULT;
		coor[index].z *= ZOOM_DEFAULT;
		//printf("x:%d y:%d z:%d\n", coor[index].x, coor[index].y, coor[index].z);
		//coor[index].x -= map.row * distance;
		//coor[index].y -= map.row * distance;
		//coor[index].z += map.column * distance;
		coor[index].screen_x = (cos(component.beta) * coor[index].x - sin(component.beta) * coor[index].z);
		coor[index].screen_y = -(sin(component.alpha) * sin(component.beta) * coor[index].x \
										  + cos(component.alpha) * coor[index].y \
							   + (sin(component.alpha) * cos(component.beta) * coor[index].z));

		//coor[index].new_x = (cos(component.beta) * coor[index].x - sin(component.beta) * coor[index].z);
		//coor[index].new_y = (sin(component.alpha) * sin(component.beta) * coor[index].x \
										  + cos(component.alpha) * coor[index].y \
							   + (sin(component.alpha) * cos(component.beta) * coor[index].z));
		//coor[index].new_z = cos(component.alpha) * sin(component.beta) * coor[index].x - sin(component.alpha) * coor[index].y + cos(component.alpha) * cos(component.beta) * coor[index].z;
		//printf("x:%f y:%f z: %f\n", coor[index].new_x, coor[index].new_y, coor[index].new_z);
		//coor[index].screen_x = coor[index].new_x;
		//coor[index].screen_y = -coor[index].new_y;
		//coor[index].screen_x *= 20;
		//coor[index].screen_y *= 20;

		//coor[index].screen_x = (cos(component.beta) * coor[index].x + sin(component.alpha) * sin(component.beta) * coor[index].y - sin(component.beta) * cos(component.beta)* coor[index].z);
		//coor[index].screen_y = sin(component.alpha) * coor[index].z + cos(component.alpha) * coor[index].y;
		
		//printf("x:%d y:%d\n", coor[index].screen_x, coor[index].screen_y);
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
		add_edge(undirect, i + map.column, i + 1, coor);
		i++;
	}
	i = 0;
	while (i + map.column < map.size)
	{
		add_edge(undirect, i , i + map.column, coor);
		i++;
	}
}

#include <errno.h>

int main(int argc, char **argv)
{
	t_graph				*undirect;
	int					fd;
	int					fd2;
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

	filename = argv[1];
	fd = open(filename, O_RDONLY);
	fd2 = open(filename, O_RDONLY);
	if (fd == -1 || fd2 == -1)
	{
		perror(filename);
		exit(1);

	}
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
	if (read_map2(fd2, coor) == -1)
	{
		perror("failed to read map.");
		exit(1);
	}
	close(fd2);
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
