#ifndef FDF_GRAPH_H
# define FDF_GRAPH_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

typedef struct	s_coordinate
{
	int		x;
	int		y;
	int		z;
	double	new_x;
	double	new_y;
	double	new_z;
	double	screen_x;
	double	screen_y;
	char	*color;
}				t_coordinate;

typedef struct	s_coordinate_list
{
	int		x;
	int		y;
	int		z;
	double	new_x;
	double	new_y;
	double	new_z;
	double	screen_x;
	double	screen_y;
	char	*color;
	t_coordinate	*block;
	struct s_coordinate_list *next;
}				t_coordinate_list;



typedef struct	s_node
{
	int	vertex_id;
	int	visited;
	double *screen_x;
	double *screen_y;
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
	int	column;
	int	row;
	int	size;
}				t_map;


t_graph	*create_graph(int max_vertex_count);

int		add_vertex(t_graph *graph, int vertex_id, t_coordinate *coor);
int		add_edge(t_graph *graph, int from_vertex_id, int to_vertex_id, t_coordinate *coor);
int		is_empty_graph(t_graph *graph);
void	display_graph(t_graph *graph);
int 	is_vertex_valid(t_graph *graph, int vertex_id);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_calloc(size_t count, size_t len);
int		read_map(int fd, t_coordinate_list **save, t_map *map);
int		read_map2(int fd, t_coordinate *save, t_map *map);
void	put_map(t_coordinate_list *coor_list, t_coordinate **coor);
void	deleteLinkedGraph(t_graph *pGraph);

#endif

#ifndef _COMMON_GRAPH_DEF_
# define _COMMON_GRAPH_DEF_

# define TRUE				1
# define FALSE				0

# define USED				1
# define NOT_USED			0

# define SUCCESS			1
# define FAIL				0

# define GRAPH_UNDIRECTED	1
# define GRAPH_DIRECTED		2

#endif
