#include "fdf_graph.h"
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
		//for (int j = 0; j < 1; j++)
		//{
			for (int k = 0; k < 3; k++)
			{
				new_point[i] += (matrix[i][k] * point[k]);
				printf("%f * %d + \n", matrix[i][k], point[k]);
			}
			printf("= %f\n", new_point[i]);
		//}
	}
	printf("\n");
	coor->new_x = new_point[0];
	coor->new_y = new_point[1];
	coor->new_z = new_point[2];
	
	printf("inside of matrix calculation: %f %f %f\n", coor->new_x, coor->new_y, coor->new_z);
	//printf("inside of matrix calculation: %d %d %d\n", coor->x, coor->y, coor->z);
	//coor->screen_x = -(coor->new_x / (coor->new_z)) * 20;
	//coor->screen_y = -(coor->new_y / (coor->new_z)) * 20;
	printf("x/z:%f/%f = %f\n", coor->new_x,coor->new_z, coor->new_x / coor->new_z);	
	printf("y/z:%f/%f = %f\n", coor->new_y,coor->new_z, coor->new_y / coor->new_z);	
	//int a = 0;
	//int b = 1;
	//printf("variable : 1 / 0 = %d\n", b / a * 20);
	//printf("직접넣음 : 1 / 0 = %d\n", 1 / 0 * 20);
	
	coor->screen_x = -coor->new_x;
	coor->screen_y = -coor->new_y;
	printf("screen_x: %f screen_y: %f\n", coor->screen_x, coor->screen_y);
	//coor->new_z = round(coor->new_z);
	//if (coor->new_z)
	/*if (round(coor->new_z))
	{
		printf("hey z is %.100f\n", coor->new_z);
		coor->screen_x /= coor->new_z;
		coor->screen_y /= coor->new_z;
	printf("x/z:%f/%f = %f\n", coor->new_x,coor->new_z, coor->new_x / coor->new_z);	
	printf("y/z:%f/%f = %f\n", coor->new_y,coor->new_z, coor->new_y / coor->new_z);	
	
	}*/
	printf("screen_x: %f screen_y: %f\n", coor->screen_x, coor->screen_y);
	//coor->screen_x = coor->x * 20;
	//coor->screen_y = coor->z * 20;
}

void	traversal_DFS_recursion(t_graph *graph, int vertex_id, t_coordinate *coor, double matrix[3][3])
{
	t_list	*closeNode;
	int index = 0;

	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	printf("%d ", vertex_id);
	//여기에 계산식
	matrix_calculation(&coor[vertex_id], matrix);
	//printf("inside of recursion: %f %f %f\n", coor[vertex_id].new_x, coor[vertex_id].new_y, coor[vertex_id].new_z);
	index++;
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
	t_list	*closeNode;
	int index = 0;

	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	printf("%d ", vertex_id);
	//여기에 계산식
	//matrix_calculation(&coor[vertex_id], matrix);
	graph->edge[vertex_id]->screen_x = coor[vertex_id].screen_x;
	graph->edge[vertex_id]->screen_y = coor[vertex_id].screen_y;
	//printf("inside of recursion: %f %f %f\n", coor[vertex_id].new_x, coor[vertex_id].new_y, coor[vertex_id].new_z);
	index++;
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
	t_list	*closeNode;
	int index = 0;
	int scale = 50;
	int offset = 300;

	if (graph->edge[vertex_id]->visited == VISITED)
		return ;
	printf("%d ", vertex_id);
	//double x = coor[vertex_id].screen_x;
	//double y = coor[vertex_id].screen_y;
	double x;
	double y;

	//t_list *next_point = graph->edge[vertex_id]->next;
	//printf("hi\n");
	/*//if (next_point)
	//{
		double x_next = next_point->screen_x;
		double y_next = next_point->screen_y;
		double delta_x = x_next - x;
		double delta_y = y_next - y;
		int pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		if (pixels)
		{
			delta_x /= pixels;
			delta_y /= pixels;
		}
		printf("%f %f %f %f\n", x, y, x_next, y_next);
		while (scale)
		{
    	//mlx_pixel_put(mlx, win, pixelX, pixelY, color);
			//printf("102\n");
			my_mlx_pixel_put(data, offset + x, offset + y, 0x00FF0000);//
    		x += delta_x;
    		y += delta_y;
    		//pixels_scaled3 -= delta_y3;
			scale--;
			printf("hi\n");
	//	}
	//}*/
		//printf("inside of recursion: %f %f %f\n", coor[vertex_id].new_x, coor[vertex_id].new_y, coor[vertex_id].new_z);
	graph->edge[vertex_id]->visited = VISITED;
	closeNode = graph->edge[vertex_id]->next;
	while (closeNode)
	{

		//if (closeNode->visited != VISITED)
		//	traversal_DFS_recursion2(graph, closeNode->vertex_id, coor, data);
		//closeNode = closeNode->next;
		//if (!closeNode)
		//	return ;
		/*if (pixels)
		{
			delta_x /= pixels;
			delta_y /= pixels;
		}*/
		scale = 50;
		int precise = 50;
		x = coor[vertex_id].screen_x * scale;
		y = coor[vertex_id].screen_y * scale;
		double x_next = closeNode->screen_x * scale;
		double y_next = closeNode->screen_y * scale;
		double delta_x = (x_next - x)/ precise;
		double delta_y = (y_next - y)/ precise;
		//double delta_x = (x_next - x);
		//double delta_y = (y_next - y);
		double pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		//printf("pixel : %d\n", pixels);
		
		//delta_x /= pixels;
		//delta_y /= pixels;
		//x = round(x);
		//y = round(y);
		printf("delta_x : %.1f delta_y : %.1f\n\n", delta_x, delta_y);
		index = 0;
		//while (round(pixels))
		while (precise)
		{
		printf("p%d:    %.1f %.1f   ->   p%d:%.1f %.1f\n", vertex_id, x, y, closeNode->vertex_id, x_next, y_next);
		printf("p%d:    %d %d   ->   p%d:%d %d\n", vertex_id, (int)x, (int)y, closeNode->vertex_id, (int)x_next, (int)y_next);
		printf("p%d:    %.1f %.1f   ->   p%d:%d %d\n\n", vertex_id, round(x), round(y), closeNode->vertex_id, (int)x_next, (int)y_next);

    	//mlx_pixel_put(mlx, win, pixelX, pixelY, color);
			//printf("102\n");
			//my_mlx_pixel_put(data, offset + x, offset + y, 0x00FF0000);//
			my_mlx_pixel_put(data, offset + round(x), offset + round(y), 0x00FF0000);//
    		x += delta_x;
    		y += delta_y;
    		//pixels_scaled3 -= delta_y3;
			//pixels--;
			index++;
			precise--;
		}
		printf("%d번 더했음\n", index);
		if (closeNode->visited != VISITED)
			traversal_DFS_recursion2(graph, closeNode->vertex_id, coor, data);
		closeNode = closeNode->next;
	}
}

int main()
{
	t_graph	*undirect;
	int	num_point = 8;
	t_coordinate *coor;

	coor = calloc(num_point, sizeof(t_coordinate));
	int num_col = 3;
	int scale = 50;

	printf("coordinate\nx y z\n");
	/*for (int i = 0; i < num_point; i++)
	{
		coor[i].x = i / num_col;
		coor[i].z = i % num_col;
		coor[i].y = 1;
		printf("%d %d %d\n", coor[i].x, coor[i].y, coor[i].z);
	}
	printf("\n");
	*/
	coor[0].x = -1;
	coor[0].y = -1;
	coor[0].z = -1;

	coor[1].x = 0;
	coor[1].y = 0;
	coor[1].z = 2;

	coor[2].x = 1;
	coor[2].y = -2;
	coor[2].z = -2;

	coor[3].x = 1;
	coor[3].y = -2;
	coor[3].z = 2;
	
	coor[4].x = 0;
	coor[4].y = 2;
	coor[4].z = 0;

	coor[5].x = 0;
	coor[5].y = 1;
	coor[5].z = 1;

	coor[6].x = 1;
	coor[6].y = 1;
	coor[6].z = 0;

	coor[7].x = 1;
	coor[7].y = 1;
	coor[7].z = 1;

		double alpha = asin(tan(30 * M_PI / 180));
	double beta = 45 * M_PI / 180;

	//asin();
	printf("alpha : %f, beta : %f, tan45 : %f\n", alpha, beta, tan(45 * M_PI / 180));
	double x_move = 0;
	double y_move = 0;
	double z_move = 0;

	double	translation_matrix[3][3] = {{cos(beta), 0, -sin(beta)}, \
		{sin(alpha) * sin(beta), cos(alpha), sin(alpha) * cos(beta)}, \
		{cos(alpha) * sin(beta), -sin(alpha), cos(alpha) * cos(beta)}}; 


	void	*mlx;
	void	*mlx_win;
	t_data	img;


	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1280, 720, "Tutorial Window");
	img.img = mlx_new_image(mlx, 1280, 720);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	int offset = 100;
	//int scale = 10;

	//make graph
	undirect = create_graph(num_point);
	t_graph *undirect2 = create_graph(num_point);
	add_vertex(undirect, 0);
	add_vertex(undirect, 1);
	add_vertex(undirect, 2);
	add_vertex(undirect, 3);
	add_vertex(undirect, 4);
	add_vertex(undirect, 5);
	add_vertex(undirect, 6);
	add_vertex(undirect, 7);
	//add_vertex(undirect, 8);
	add_edge(undirect, 0, 1);
	add_edge(undirect, 0, 2);
	//add_edge(undirect, 2, 3);
	//add_edge(undirect, 3, 1);
	add_edge(undirect, 4, 5);
	add_edge(undirect, 4, 6);
	add_edge(undirect, 6, 7);
	add_edge(undirect, 5, 7);
	add_edge(undirect, 0, 4);
	add_edge(undirect, 1, 5);
	add_edge(undirect, 2, 6);
	//add_edge(undirect, 3, 7);
	printf("diplay undirected graph\n");
	display_graph(undirect);
	printf("DFS recursion\n");
	traversal_DFS_recursion(undirect, 0, coor, translation_matrix);
	printf("\nnew points\n");
	for (int i = 0; i < num_point; i++)
	{
		printf("%f %f\n", coor[i].screen_x, coor[i].screen_y);
		undirect->edge[i]->screen_x = coor[i].screen_x;
		undirect->edge[i]->screen_y = coor[i].screen_y;
	}
	printf("\n");

	for(int i = 0; i < undirect->max_vertex_count; i++)
		undirect->edge[i]->visited = 0;
	for(int i = 0; i < undirect->max_vertex_count; i++)
	{
		printf("%f %f\n", undirect->edge[i]->screen_x, undirect->edge[i]->screen_y);
	}
	/*
	add_vertex(undirect2, 0);
	add_vertex(undirect2, 1);
	add_vertex(undirect2, 2);
	add_vertex(undirect2, 3);
	add_vertex(undirect2, 4);
	add_vertex(undirect2, 5);
	add_vertex(undirect2, 6);
	add_vertex(undirect2, 7);
	add_vertex(undirect2, 8);
	add_edge(undirect2, 0, 1);
	add_edge(undirect2, 0, 3);
	add_edge(undirect2, 1, 2);
	add_edge(undirect2, 1, 4);
	add_edge(undirect2, 2, 5);
	add_edge(undirect2, 3, 6);
	add_edge(undirect2, 3, 4);
	add_edge(undirect2, 4, 5);
	add_edge(undirect2, 4, 7);
	add_edge(undirect2, 6, 7);
	add_edge(undirect2, 8, 5);
	add_edge(undirect2, 8, 7);
	*/
	traversal_DFS_recursion1(undirect, 0, coor);
	display_graph(undirect);
	for(int i = 0; i < undirect->max_vertex_count; i++)
		undirect->edge[i]->visited = 0;
	traversal_DFS_recursion2(undirect, 0, coor, &img);
	printf("\n\n");
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
}
