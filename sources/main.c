#include "../includes/fdf.h"
#include "../mlx/mlx.h"
#include "../libft/libft.h"
#include "../includes/error_messages.h"

int	main(int argc, char **argv)
{
	t_graph				*graph;
	t_file				file;
	t_data				data;
	t_map				map;

	if (argc != 3)
		terminate(ERR_ARGUMENTS);
	file.name = argv[1];
	file.fd = open(file.name, O_RDONLY);
	if (file.fd == -1)
		terminate(file.name);
	init_map(&map);
	map.scale = ft_atoi(argv[2]);
	if (map.scale == 0)
		map.scale = ZOOM_DEFAULT;
	set_map(&map, &file);
	set_mlx(&data, &map);
	graph = create_graph(map.size);
	connect_vertexes(graph, &map, map.coor);
	draw_dfs(graph, 0, &data);
	mlx_put_image_to_window(data.mlx, data.mlx_win, data.img, 0, 0);
	delete_graph(graph);
	mlx_hook(data.mlx_win, X_EVENT_KEY_PRESS, 0, &key_press, 0);
	mlx_loop(data.mlx);
	return (0);
}
