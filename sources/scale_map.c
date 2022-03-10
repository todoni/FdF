#include "../includes/fdf.h"

static void	resize_y(t_coordinate *coor, int size, double scale)
{
	int	index;

	index = 0;
	while (index < size)
	{
		coor[index].screen_y *= scale;
		index++;
	}
}

static void	resize_x(t_coordinate *coor, int size, double scale)
{
	int	index;

	index = 0;
	while (index < size)
	{
		coor[index].screen_x *= scale;
		index++;
	}
}

void	scale_map(t_map *map)
{
	if (map->screen_height > MAX_SCREEN_HEIGHT)
	{
		map->scale = MAX_SCREEN_HEIGHT / (double)map->screen_height;
		resize_y(map->coor, map->size, map->scale);
		resize_x(map->coor, map->size, map->scale);
		map->screen_height = MAX_SCREEN_HEIGHT;
		map->screen_width = find_x_max(map->coor, map->size);
	}
	if (map->screen_width > MAX_SCREEN_WIDTH)
	{
		map->scale = MAX_SCREEN_WIDTH / (double)map->screen_width;
		resize_x(map->coor, map->size, map->scale);
		resize_y(map->coor, map->size, map->scale);
		map->screen_width = MAX_SCREEN_WIDTH;
		map->screen_height = ft_abs(find_y_max(map->coor, map->size) \
									- find_y_min(map->coor, map->size));
	}
}
