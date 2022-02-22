#include "fdf_graph.h"
#include "libft/libft.h"
#include "mlx.h"
#include <stdio.h>
#include <math.h>



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
	double	x_move;
	double	y_move;
	double	z_move;
}				t_matrix;


void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	matrix_calculation(t_coordinate *coor, double matrix[][4], t_matrix component)
{
	int 	point[4] = {coor->x * ZOOM_DEFAULT, coor->y * ZOOM_DEFAULT, coor->z * ZOOM_DEFAULT, 1};
	double 	new_point[4] = {0,};
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (i < 3)
	{
			j = 0;
			while (j < 3)
			{
				new_point[i] += (matrix[i][j] * point[j]);
				j++;
			}
			i++;
	}
	coor->new_x = new_point[0];
	coor->new_y = new_point[1];
	coor->new_z = new_point[2];
	coor->screen_x = -coor->new_x;
	coor->screen_y = -coor->new_y;
	coor->screen_x += component.x_move;
	coor->screen_y += component.y_move;
	//printf("x  y\n%.1f %.1f\n", coor->screen_x, coor->screen_y);
}

void	traversal_DFS_recursion(t_graph *graph, int vertex_id, t_coordinate *coor, double matrix[][4], t_matrix component)
{
	t_node	*closeNode;

	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	matrix_calculation(&coor[vertex_id], matrix, component);
	graph->edge[vertex_id]->visited = VISITED;
	closeNode = graph->edge[vertex_id]->next;
	while (closeNode)
	{	
		if (closeNode->visited != VISITED)
			traversal_DFS_recursion(graph, closeNode->vertex_id, coor, matrix, component);
		closeNode = closeNode->next;
	}
}

/*void	traversal_DFS_recursion1(t_graph *graph, int vertex_id, t_coordinate *coor)
{
	t_node	*closeNode;

	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	graph->edge[vertex_id]->screen_x = coor[vertex_id].screen_x;
	graph->edge[vertex_id]->screen_y = coor[vertex_id].screen_y;
	graph->edge[vertex_id]->visited = VISITED;
	closeNode = graph->edge[vertex_id]->next;
	while (closeNode)
	{	
		int next_id = closeNode->vertex_id;
		closeNode->screen_x = coor[next_id].screen_x;
		closeNode->screen_y = coor[next_id].screen_y;
		if (closeNode->visited != VISITED)
			traversal_DFS_recursion1(graph, closeNode->vertex_id, coor);
		closeNode = closeNode->next;
	}
}*/

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

	//scale = 10;
	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	graph->edge[vertex_id]->visited = VISITED;
	closeNode = graph->edge[vertex_id]->next;
	while (closeNode)
	{
		x = *graph->edge[vertex_id]->screen_x; //* scale;
		y = *graph->edge[vertex_id]->screen_y; //* scale;
		x_next = *(closeNode->screen_x); //* scale;
		y_next = *(closeNode->screen_y); //* scale;
		delta_x = (x_next - x);
		delta_y = (y_next - y);
		pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		
		delta_x /= pixels;
		delta_y /= pixels;
		printf("%d\n", vertex_id);
		while (round(pixels))
		{
			my_mlx_pixel_put(data, (offset.x / 2) + round(x), offset.y + round(y), 0x00FF0000);
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

void	make_coor(t_coordinate *coor, t_coordinate_list *save)
{
	int	index;

	index = 0;
	while (save)
	{
		coor[index].x = save->x;
		coor[index].y = save->y;
		coor[index].z = save->z;
		index++;
		save = save->next;
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
#define X_EVENT_KEY_release		3
#define X_EVENT_KEY_EXIT		17 //exit key code

//Mac key code example
//All the key code example other than below is described on the site linked in READEME.md
#define KEY_ESC			53
#define KEY_UP			126
#define KEY_DOWN		125
#define KEY_RIGHT		124
#define KEY_LEFT		123
#define SCREEN_MAX_WIDTH 1440
#define SCREEN_MAX_HEIGHT 900

typedef struct s_multi
{
	t_coordinate *coor;
	t_graph *undirect;
	t_matrix component;
	t_data	img;
	void	*mlx;
	void	*mlx_win;
	t_offset	offset;
	double	translation_matrix[4][4];
	int		screen_max;
	int		screen_min;
}			t_param;


void	make_matrix(double matrix[][4], t_matrix component)
{
	matrix[0][0] = cos(component.beta);
	matrix[0][1] = 0;
	matrix[0][2] = -sin(component.beta);
	matrix[1][0] = sin(component.alpha) * sin(component.beta);
	matrix[1][1] = cos(component.alpha);
	matrix[1][2] = sin(component.alpha) * cos(component.beta);
	matrix[2][0] = cos(component.alpha) * sin(component.beta);
	matrix[2][1] = -sin(component.alpha);
	matrix[2][2] = cos(component.alpha) * cos(component.beta);
	matrix[0][3] = component.x_move;
	matrix[1][3] = component.y_move;
	matrix[2][3] = component.z_move;
	matrix[3][3] = 1;
	matrix[3][0] = 0;
	matrix[3][1] = 0;
	matrix[3][2] = 0;
}


void	matrix_calc(t_coordinate *coor, double matrix[][4], t_matrix component, int	size)
{
	int		index;
	int 	point[4];
	double 	new_point[4];
	int		i;
	int		j;
	int		k;
	int		sizee;
	double	cos_alpha;
	double	cos_beta;
	double	sin_alpha;
	double	sin_beta;

	cos_alpha = cos(component.alpha);
	cos_beta = cos(component.beta);
	sin_alpha = sin(component.alpha);
	sin_beta = sin(component.beta);
	index = 0;
	k = 0;
	sizee = sizeof(t_coordinate);
	/*while (index < size)
	{
		point[0] = coor[index].x * ZOOM_DEFAULT;
		point[1] = coor[index].y * ZOOM_DEFAULT;
		point[2] = coor[index].z * ZOOM_DEFAULT;
		new_point[0] = 0;
		new_point[1] = 0;
		new_point[2] = 0;
		i = 0;
		j = 0;
		while (i < 3)
		{
			j = 0;
			while (j < 3)
			{
				new_point[i] += (matrix[i][j] * point[j]);
				j++;
			}
			i++;
		}
		coor[index].screen_x = -new_point[0];
		coor[index].screen_y = -new_point[1];
		coor[index].screen_x += component.x_move;
		coor[index].screen_y += component.y_move;
		index++;
	}*/
	while (index < size)
	{
		coor[index].x *= ZOOM_DEFAULT;
		coor[index].y *= ZOOM_DEFAULT;
		coor[index].z *= ZOOM_DEFAULT;
		coor[index].screen_x = (cos_beta * coor[index].x + sin_beta * coor[index].z);
		coor[index].screen_y = (sin_alpha * sin_beta * coor[index].x \
										  + cos_alpha * coor[index].y \
							   - sin_alpha * cos_beta * coor[index].z);
		coor[index].screen_x += component.x_move;
		coor[index].screen_y += component.y_move;
		index++;
	}

	printf("x  y\n%.1f %.1f\n", coor->screen_x, coor->screen_y);
}


int	key_press(int keycode, t_param *param)
{
	if (keycode == KEY_ESC)
		exit(0);
	else if (keycode == KEY_DOWN)
	{
		param->component.y_move++;
		make_matrix(param->translation_matrix, param->component);
		matrix_calc(param->coor, param->translation_matrix, param->component, param->undirect->current_vertex_count);
		//mlx_destroy_image(param->mlx, &param->img);
		param->img.img = mlx_new_image(param->mlx, (param->screen_max) * 2, (param->screen_min) * 2);
		param->img.addr = mlx_get_data_addr(param->img.img, &param->img.bits_per_pixel, &param->img.line_length, &param->img.endian);
		traversal_DFS_recursion2(param->undirect, 0, &param->img, param->offset);
		initialize_visit(param->undirect);
		mlx_clear_window(param->mlx, param->mlx_win);
		mlx_put_image_to_window(param->mlx, param->mlx_win, param->img.img, 0, 0);
	}
	else if (keycode == KEY_RIGHT)
	{	
		param->component.x_move++;
		make_matrix(param->translation_matrix, param->component);
		matrix_calc(param->coor, param->translation_matrix, param->component, param->undirect->current_vertex_count);
		param->img.img = mlx_new_image(param->mlx, (param->screen_max) * 2, (param->screen_min) * 2);
		param->img.addr = mlx_get_data_addr(param->img.img, &param->img.bits_per_pixel, &param->img.line_length, &param->img.endian);
		traversal_DFS_recursion2(param->undirect, 0, &param->img, param->offset);
		initialize_visit(param->undirect);
		mlx_clear_window(param->mlx, param->mlx_win);
		mlx_put_image_to_window(param->mlx, param->mlx_win, param->img.img, 0, 0);
	}
	else if (keycode == KEY_LEFT)
	{	
		param->component.x_move--;
		make_matrix(param->translation_matrix, param->component);
		matrix_calc(param->coor, param->translation_matrix, param->component, param->undirect->current_vertex_count);
		param->img.img = mlx_new_image(param->mlx, (param->screen_max) * 2, (param->screen_min) * 2);
		param->img.addr = mlx_get_data_addr(param->img.img, &param->img.bits_per_pixel, &param->img.line_length, &param->img.endian);
		traversal_DFS_recursion2(param->undirect, 0, &param->img, param->offset);
		initialize_visit(param->undirect);
		mlx_clear_window(param->mlx, param->mlx_win);
		mlx_put_image_to_window(param->mlx, param->mlx_win, param->img.img, 0, 0);
	}
	else if (keycode == KEY_UP)
	{	
		param->component.y_move--;
		make_matrix(param->translation_matrix, param->component);
		matrix_calc(param->coor, param->translation_matrix, param->component, param->undirect->current_vertex_count);
		param->img.img = mlx_new_image(param->mlx, (param->screen_max) * 2, (param->screen_min) * 2);
		param->img.addr = mlx_get_data_addr(param->img.img, &param->img.bits_per_pixel, &param->img.line_length, &param->img.endian);
		traversal_DFS_recursion2(param->undirect, 0, &param->img, param->offset);
		initialize_visit(param->undirect);
		mlx_clear_window(param->mlx, param->mlx_win);
		mlx_put_image_to_window(param->mlx, param->mlx_win, param->img.img, 0, 0);
	}


	printf("x->%.0f y->%.0f\n", param->component.x_move, param->component.y_move);
	return (0);
}

void	resize()
{

}

void	connect_vertexes(t_graph *undirect, int point_per_line, int num_point, t_coordinate *coor)
{
	int i;

	i = 0;
	while (i < num_point)
	{
		add_vertex(undirect, i, coor);
		i++;
	}
	i = 0;
	while (i < num_point)
	{
		if ((i + 1) % point_per_line == 0)
			i++;
		add_edge(undirect, i, i + 1, coor);
		i++;
	}
	i = 0;
	while (i + point_per_line < num_point)
	{
		add_edge(undirect, i, i + point_per_line, coor);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_graph				*undirect;
	int					num_point;
	int					fd;
	int					point_per_line;
	t_coordinate		coor[1000000];
	t_coordinate_list	*save;
	double				alpha;
	double				beta;
	double				x_move;
	double				y_move;
	double				z_move;
	void				*mlx;
	void				*mlx_win;
	t_data				img;
	int					screen_max;
	int					screen_min;
	double				translation_matrix[4][4];
	t_matrix			component;
	t_offset			offset;
	char				*filename;
	t_param				param;
	t_map				map;

	filename = argv[1];

	fd = open(filename, O_RDONLY);
	save = 0;
	//point_per_line = read_map(fd, &save);
	point_per_line = read_map2(fd, coor, &map);
	//num_point = count_size(save);
	num_point = map.size;
	//coor = (t_coordinate *)ft_calloc(num_point, sizeof(t_coordinate));
	//make_coor(coor, save);
	component.alpha = asin(tan(30 * M_PI / 180));
	component.beta = 45 * M_PI / 180;

	component.x_move = 0;
	component.y_move = 0;
	component.z_move = 0;

	//double	translation_matrix[3][3] = {{cos(beta), 0, -sin(beta)}, \
		{sin(alpha) * sin(beta), cos(alpha), sin(alpha) * cos(beta)}, \
		{cos(alpha) * sin(beta), -sin(alpha), cos(alpha) * cos(beta)}}; 
	mlx = mlx_init();
	make_matrix(translation_matrix, component);
	undirect = create_graph(num_point);
	t_graph *undirect2 = create_graph(num_point);
	display_graph(undirect);
	connect_vertexes(undirect, point_per_line, num_point, coor);
	display_graph(undirect);
	//coor[1].screen_x = 3;
	//coor[1].screen_y = 3;
	//display_graph(undirect);
	//traversal_DFS_recursion(undirect, 0, coor, translation_matrix, component);
	matrix_calc(coor, translation_matrix, component, num_point);
	printf("hi%d %d %d\n", coor[1].x, coor[1].y, coor[1].z);
	display_graph(undirect);
	initialize_visit(undirect);
	//traversal_DFS_recursion1(undirect, 0, coor);
	//initialize_visit(undirect);
	screen_max = find_x_max(coor, num_point);
	//if (screen_max > SCREEN_MAX_WIDTH)
	//	screen_max = SCREEN_MAX_WIDTH;
	screen_min = abs(find_y_min(coor, num_point)) + abs(find_y_max(coor, num_point));
	//if (screen_min > SCREEN_MAX_HEIGHT)
	//	screen_min = SCREEN_MAX_HEIGHT;
	printf("%d %d\n", screen_max, screen_min);
	offset.x = abs(find_x_max(coor, num_point));
	offset.y = abs(find_y_min(coor, num_point)) + (screen_min / 3);
	img.img = mlx_new_image(mlx, screen_max * 2, screen_min * 2);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	mlx_win = mlx_new_window(mlx, screen_max * 2, screen_min * 2, "fdf");
	//mlx_win = mlx_new_window(mlx, 1440, 900, "fdf");
	traversal_DFS_recursion2(undirect, 0, &img, offset);
	printf("done\n");
	initialize_visit(undirect);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	param.component = component;
	param.coor = coor;
	param.img = img;
	param.mlx = mlx;
	param.mlx_win = mlx_win;
	param.offset = offset;
	param.screen_max = screen_max;
	param.screen_min = screen_min;
	//param.translation_matrix = make_matrix(translation_matrix, component);
	param.undirect = undirect;
	mlx_hook(mlx_win, X_EVENT_KEY_PRESS, 0, &key_press, &param);
	mlx_loop(mlx);
	printf("hi\n");
}
