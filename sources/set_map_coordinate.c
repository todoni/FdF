/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map_coordinate.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 15:35:04 by sohan             #+#    #+#             */
/*   Updated: 2022/03/11 15:35:12 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"
#include "../includes/error_messages.h"
#include "../libft/libft.h"
#define ZOOM_DEFAULT 64

static int	calculate_screen_coordinate(t_map *map)
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
	return (index);
}

static char	*extract_color(char *str)
{
	char	**color;
	char	*target;

	target = 0;
	color = ft_split(str, ',');
	if (!color)
		terminate(ERR_MAP_READING);
	if (color[1])
		target = ft_strdup(color[1]);
	free_array(color);
	return (target);
}

static int	put_map_color(t_list *lst, t_coordinate *coor)
{
	char	**block;
	int		i;
	int		j;

	j = 0;
	while (lst)
	{
		block = lst->content;
		i = 0;
		while (block[i])
		{
			coor[j].color = extract_color(block[i]);
			i++;
			j++;
		}
		lst = lst->next;
	}
	return (j);
}

static int	put_map_xyz(t_list *lst, t_coordinate *coor)
{
	char			**block;
	int				i;
	int				j;
	int				row;

	j = 0;
	row = 0;
	while (lst)
	{
		block = lst->content;
		i = 0;
		while (block[i])
		{
			coor[j].x = row;
			coor[j].y = ft_atoi(block[i]);
			coor[j].z = i;
			i++;
			j++;
		}
		row++;
		lst = lst->next;
	}
	return (j);
}

void	set_map_coordinate(t_map *map)
{
	char	**block;

	block = map->list_coordinate->content;
	if (!block[0])
		map->list_coordinate = map->list_coordinate->next;
	map->coor = ft_calloc(map->size, sizeof(t_coordinate));
	if (!map->coor)
		terminate(ERR_MEMORY_ALLOCATION);
	put_map_xyz(map->list_coordinate, map->coor);
	put_map_color(map->list_coordinate, map->coor);
	calculate_screen_coordinate(map);
}
