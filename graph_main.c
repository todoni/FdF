#include "fdf_graph.h"
#include "libft/libft.h"
#include "mlx.h"
#include <stdio.h>
#include <math.h>



#define VISITED 1


typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	matrix_calculation(t_coordinate *coor, double matrix[3][3])
{
	int point[3] = {coor->x, coor->y, coor->z};
	double new_point[3] = {0,};
	double mul = 0;

	for (int i = 0; i < 3; i++)
	{
			for (int k = 0; k < 3; k++)
			{
				new_point[i] += (matrix[i][k] * point[k]);
			}
	}
	coor->new_x = new_point[0];
	coor->new_y = new_point[1];
	coor->new_z = new_point[2];
	
	coor->screen_x = -coor->new_x;
	coor->screen_y = -coor->new_y;
}

void	traversal_DFS_recursion(t_graph *graph, int vertex_id, t_coordinate *coor, double matrix[3][3])
{
	t_node	*closeNode;

	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	matrix_calculation(&coor[vertex_id], matrix);
	graph->edge[vertex_id]->visited = VISITED;
	closeNode = graph->edge[vertex_id]->next;
	while (closeNode)
	{	
		if (closeNode->visited != VISITED)
			traversal_DFS_recursion(graph, closeNode->vertex_id, coor, matrix);
		closeNode = closeNode->next;
	}
}

void	traversal_DFS_recursion1(t_graph *graph, int vertex_id, t_coordinate *coor)
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
}

void	traversal_DFS_recursion2(t_graph *graph, int vertex_id, t_coordinate *coor, t_data *data)
{
	t_node	*closeNode;
	int		scale;
	int		offset;
	double	x;
	double	y;
	double	pixels;
	double	x_next;
	double	y_next;
	double	delta_x;
	double	delta_y;

	offset = 300;
	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	graph->edge[vertex_id]->visited = VISITED;
	closeNode = graph->edge[vertex_id]->next;
	while (closeNode)
	{
		scale = 10;
		x = coor[vertex_id].screen_x * scale;
		y = coor[vertex_id].screen_y * scale;
		x_next = closeNode->screen_x * scale;
		y_next = closeNode->screen_y * scale;
		delta_x = (x_next - x);
		delta_y = (y_next - y);
		pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		
		delta_x /= pixels;
		delta_y /= pixels;
		while (round(pixels))
		{
			my_mlx_pixel_put(data, offset + round(x), offset + round(y), 0x00FF0000);
    		x += delta_x;
    		y += delta_y;
			pixels--;
		}
		if (closeNode->visited != VISITED)
			traversal_DFS_recursion2(graph, closeNode->vertex_id, coor, data);
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

void	connect_vertexes(t_graph *undirect, int point_per_line, int num_point)
{
	int i;

	i = 0;
	while (i < num_point)
	{
		add_vertex(undirect, i);
		i++;
	}
	i = 0;
	while (i < num_point)
	{
		if ((i + 1) % point_per_line == 0)
			i++;
		add_edge(undirect, i, i + 1);
		i++;
	}
	i = 0;
	while (i + point_per_line < num_point)
	{
		add_edge(undirect, i, i + point_per_line);
		i++;
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

#define X_EVENT_KEY_PRESS		2
#define X_EVENT_KEY_release		3
#define X_EVENT_KEY_EXIT		17 //exit key code

//Mac key code example
//All the key code example other than below is described on the site linked in READEME.md
#define KEY_ESC			53

int	key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

int main()
{
	t_graph				*undirect;
	int					num_point;
	int					fd;
	int					point_per_line;
	t_coordinate		*coor;
	t_coordinate_list	*save;
	double				alpha;
	double				beta;
	double				x_move;
	double				y_move;
	double				z_move;
	void				*mlx;
	void				*mlx_win;
	t_data				img;

	fd = open("/Users/sohan/42cursus/fdf/test_maps/42.fdf", O_RDONLY);
	save = 0;
	point_per_line = read_map(fd, &save);
	num_point = count_size(save);
	coor = (t_coordinate *)ft_calloc(num_point, sizeof(t_coordinate));
	make_coor(coor, save);
	alpha = asin(tan(-30 * M_PI / 180));
	beta = M_PI - (45 * M_PI / 180);

	x_move = 0;
	y_move = 0;
	z_move = 0;

	double	translation_matrix[3][3] = {{cos(beta), 0, -sin(beta)}, \
		{sin(alpha) * sin(beta), cos(alpha), sin(alpha) * cos(beta)}, \
		{cos(alpha) * sin(beta), -sin(alpha), cos(alpha) * cos(beta)}}; 

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1280, 720, "Tutorial Window");
	img.img = mlx_new_image(mlx, 1280, 720);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	undirect = create_graph(num_point);
	t_graph *undirect2 = create_graph(num_point);
	connect_vertexes(undirect, point_per_line, num_point);
	traversal_DFS_recursion(undirect, 0, coor, translation_matrix);
	initialize_visit(undirect);
	traversal_DFS_recursion1(undirect, 0, coor);
	initialize_visit(undirect);
	traversal_DFS_recursion2(undirect, 0, coor, &img);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_hook(mlx_win, X_EVENT_KEY_PRESS, 0, &key_press, 0);
	mlx_loop(mlx);
}
