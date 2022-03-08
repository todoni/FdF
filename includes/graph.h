#ifndef GRAPH_H
# define GRAPH_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft.h"

# define TRUE		1
# define FALSE		0

# define USED		1
# define NOT_USED	0

# define SUCCESS	1
# define FAIL		0

# define VISITED	1

typedef struct	s_coordinate
{
	int		x;
	int		y;
	int		z;
	int		screen_x;
	int		screen_y;
	char	*color;
}				t_coordinate;

typedef struct	s_node
{
	int		vertex_id;
	int		visited;
	int		*screen_x;
	int		*screen_y;
	char	*color;
	struct s_node *next;
}				t_node;

typedef struct	s_graph
{
	int		max_vertex_count;
	int		current_vertex_count;
	int		current_edge_count;
	t_node	**edge;
	int		*vertex;
}				t_graph;

typedef struct	s_map
{
	int				column;
	int				row;
	int				size;
	int				screen_width;
	int				screen_height;
	double			scale;
	double			rotation_x;
	double			rotation_y;
	t_list			*block;
	t_coordinate	*coor;
}				t_map;

t_graph	*create_graph(int max_vertex_count);
int		add_vertex(t_graph *graph, int vertex_id, t_coordinate *coor);
int		add_edge(t_graph *graph, int from_vertex_id, int to_vertex_id, t_coordinate *coor);
int 	is_vertex_valid(t_graph *graph, int vertex_id);
void	deleteLinkedGraph(t_graph *pGraph);
void	terminate(char *error_message);
t_list	*read_map(int fd, t_map *map);
int		get_map_size(t_list *lst, t_map *map);
int		make_coordinate(t_list *lst, t_coordinate *coor);
int		get_map_color(t_list *lst, t_coordinate *coor);

#endif
