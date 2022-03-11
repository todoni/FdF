#include "../includes/fdf.h"
#include "../includes/error_messages.h"
#include "../mlx/mlx.h"

int	key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
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

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}
