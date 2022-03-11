#ifndef FDF_H
# define FDF_H

# include "../libft/libft.h"
# include "math.h"
# include <fcntl.h>

# define X_EVENT_KEY_PRESS		2
# define KEY_ESC				53
# define MAX_SCREEN_WIDTH 		1440
# define MAX_SCREEN_HEIGHT 		900

# define TRUE			1
# define FALSE			0

# define USED			1
# define NOT_USED		0

# define SUCCESS		1
# define FAIL			0

# define VISITED		1

typedef struct s_data
{
	void	*mlx;
	void	*mlx_win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_coordinate
{
	int		x;
	int		y;
	int		z;
	int		screen_x;
	int		screen_y;
	char	*color;
}				t_coordinate;

typedef struct s_map
{
	int				column;
	int				row;
	int				size;
	int				screen_width;
	int				screen_height;
	double			scale;
	double			rotation_x;
	double			rotation_y;
	t_list			*list_coordinate;
	t_coordinate	*coor;
}	t_map;

typedef struct s_color_gradient
{
	int	from[3];
	int	to[3];
	int	delta[3];
	int	sum[3];
}	t_gradient;

typedef struct s_bresenham
{
	int	x;
	int	y;
	int	x_next;
	int	y_next;
	int	delta_x;
	int	delta_y;
	int	add_x;
	int	add_y;
	int	err;
	int	e2;
	int	distance;
}	t_bresenham;

typedef struct s_node
{
	int				vertex_id;
	int				visited;
	int				*screen_x;
	int				*screen_y;
	char			*color;
	struct s_node	*next;
}	t_node;

typedef struct s_graph
{
	int		max_vertex_count;
	int		current_vertex_count;
	int		current_edge_count;
	t_node	**edge;
	int		*vertex;
}	t_graph;

typedef struct s_file
{
	int		fd;
	char	*name;
}	t_file;

t_graph	*create_graph(int max_vertex_count);
int		add_vertex(t_graph *graph, int vertex_id, t_coordinate *coor);
int		add_edge(t_graph *graph, int from_vertex_id, \
								int to_vertex_id, t_coordinate *coor);
int		connect_vertex(t_graph *graph, t_map *map, t_coordinate *coor);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	init_color(t_gradient *dt);
void	set_pixel_color(int *color, char *rgb);
void	get_delta_color(t_gradient *dt);
void	set_color_gradient(t_gradient *dt);
int		find_x_max(t_coordinate *coor, int num_point);
int		find_y_min(t_coordinate *coor, int num_point);
int		find_y_max(t_coordinate *coor, int num_point);
void	terminate(char *error_message);
void	free_array(char **ptr);
t_list	*read_map(int fd, t_map *map);
void	set_map_coordinate(t_map *map);
void	adjust_map(t_map *map);
void	set_mlx(t_data *data, t_map *map);
void	draw_map(t_graph *graph, int vertex_id, t_data *data);
int		key_press(int keycode);
void	init_map(t_map *map);

#endif
