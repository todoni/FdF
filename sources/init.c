#include "../includes/fdf.h"

void	init_color(t_gradient *dt)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		dt->sum[i] = 0;
		i++;
	}
	dt->from[0] = (0xFF0000 & 0b111111110000000000000000) >> 16;
	dt->from[1] = (0xFF0000 & 0b000000001111111100000000) >> 8;
	dt->from[2] = 0xFF0000 & 0b000000000000000011111111;
	dt->to[0] = (0xFF0000 & 0b111111110000000000000000) >> 16;
	dt->to[1] = (0xFF0000 & 0b000000001111111100000000) >> 8;
	dt->to[2] = 0xFF0000 & 0b000000000000000011111111;
}

void	init_map(t_map *map)
{
	map->column = 0;
	map->row = 0;
	map->size = 0;
	map->screen_width = 0;
	map->screen_height = 0;
	map->scale = 0;
	map->rotation_x = asin(tan(26.565 * M_PI / 180));
	map->rotation_y = -45 * M_PI / 180;
	map->coor = 0;
}
