#include "../includes/fdf.h"
#include "../mlx/mlx.h"
#include "../libft/libft.h"
#include "../includes/error_messages.h"
#include <unistd.h>

int	main(int argc, char **argv)
{
	t_graph				*graph;
	t_file				file;
	t_data				data;
	t_map				map;

	if (argc != 2)
		terminate(ERR_ARGUMENTS);
	file.name = argv[1];
	file.fd = open(file.name, O_RDONLY);
	if (file.fd == -1)
		terminate(file.name);
	init_map(&map);
	map.list_coordinate = read_map(file.fd, &map);
	close(file.fd);
	set_map_coordinate(&map);
	adjust_map(&map);
	set_mlx(&data, &map);
	graph = create_graph(map.size);
	if (!graph || connect_vertex(graph, &map, map.coor) == FAIL)
		terminate(ERR_GRAPH_OPERATION);
	draw_map(graph, 0, &data);
	mlx_put_image_to_window(data.mlx, data.mlx_win, data.img, 5, 5);
	mlx_hook(data.mlx_win, X_EVENT_KEY_PRESS, 0, &key_press, 0);
	mlx_loop(data.mlx);
	return (0);
}
