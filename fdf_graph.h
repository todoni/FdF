#ifndef FDF_GRAPH_H
# define FDF_GRAPH_H

# include <unistd.h>
# include <stdlib.h>

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
}				t_coordinate;

typedef struct	s_list
{
	int	vertex_id;
	int	visited;
	double screen_x;
	double screen_y;
	struct s_list *next;
}				t_list;

typedef struct	s_graph
{
	int		max_vertex_count;
	int		current_vertex_count;
	int		current_edge_count;
	t_list	**edge;
	int		*vertex;
}				t_graph;

t_graph	*create_graph(int max_vertex_count);

int		add_vertex(t_graph *graph, int vertex_id);
int		add_edge(t_graph *graph, int from_vertex_id, int to_vertex_id);
int		is_empty_graph(t_graph *graph);
void	display_graph(t_graph *graph);
int 	is_vertex_valid(t_graph *graph, int vertex_id);

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
