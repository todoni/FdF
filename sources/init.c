#include "../includes/fdf.h"
#include "../includes/error_messages.h"

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

int	get_map_size(t_list *lst, t_map *map)
{
	int		i;
	char	**block;

	i = 0;
	while (lst)
	{
		block = lst->content;
		i = 0;
		while (block[i])
		{
			i++;
			map->size++;
		}
		lst = lst->next;
	}
	if (map->size % map->row != 0)
		terminate(ERR_MAP_INVALID);
	map->column = map->size / map->row;
	return (map->size);
}

void	jump_empty_coordinate(t_list **lst)
{
	char	**block;

	block = (*lst)->content;
	if (!block[0])
		*lst = (*lst)->next;
}

int	make_coordinate(t_list *lst, t_coordinate *coor)
{
	char			**block;
	int				i;
	int				j;
	int				row;

	j = 0;
	row = 0;
	jump_empty_coordinate(&lst);
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
	return (0);
}
