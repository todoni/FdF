/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 15:33:28 by sohan             #+#    #+#             */
/*   Updated: 2022/03/11 15:33:32 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	scale_map(t_map *map)
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

static void	set_offset(t_map *map)
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

void	adjust_map(t_map *map)
{
	map->screen_width = ft_abs(find_x_max(map->coor, map->size));
	map->screen_height = ft_abs(find_y_min(map->coor, map->size) - \
										find_y_max(map->coor, map->size));
	scale_map(map);
	set_offset(map);
}
