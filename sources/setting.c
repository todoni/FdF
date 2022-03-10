#include "../includes/fdf.h"
#include "../mlx/mlx.h"
#include "../includes/error_messages.h"
#include <unistd.h>


void	set_screen_coordinate(t_map *map)
{
	int		index;

	index = 0;
	while (index < map->size)
	{
		map->coor[index].x *= ZOOM_DEFAULT;
		map->coor[index].y *= ZOOM_DEFAULT;
		map->coor[index].z *= ZOOM_DEFAULT;
		map->coor[index].screen_x = \
								(cos(map->rotation_y) * map->coor[index].x \
								- sin(map->rotation_y) * map->coor[index].z);
		map->coor[index].screen_y = -(sin(map->rotation_x) * \
								sin(map->rotation_y) * map->coor[index].x \
								+ cos(map->rotation_x) * map->coor[index].y \
							   + (sin(map->rotation_x) * cos(map->rotation_y) \
							   						 * map->coor[index].z));
		index++;
	}
}

void	set_offset(t_map *map)
{
	int	index;
	int	offset;

	index = 0;
	offset = ft_abs(find_y_min(map->coor, map->size));
	while (index < map->size)
	{
		map->coor[index].screen_y += offset;
		index++;
	}
}

void	set_mlx(t_data *data, t_map *map)
{
	data->mlx = mlx_init();
	data->img = mlx_new_image(data->mlx, map->screen_width + 10, \
											map->screen_height + 10);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel, \
									&data->line_length, &data->endian);
	data->mlx_win = mlx_new_window(data->mlx, map->screen_width + 10, \
										map->screen_height + 10, "fdf");
}

void	get_screen_size(t_map *map)
{
	map->screen_width = ft_abs(find_x_max(map->coor, map->size));
	map->screen_height = ft_abs(find_y_min(map->coor, map->size) - \
										find_y_max(map->coor, map->size));
}

void	set_map(t_map *map, t_file *file)
{
	map->block = read_map(file->fd, map);
	get_map_size(map->block, map);
	close(file->fd);
	map->coor = ft_calloc(map->size, sizeof(t_coordinate));
	if (!map->coor)
		terminate(ERR_MEMORY_ALLOCATION);
	make_coordinate(map->block, map->coor);
	get_map_color(map->block, map->coor);
	set_screen_coordinate(map);
	get_screen_size(map);
	scale_map(map);
	set_offset(map);
}
