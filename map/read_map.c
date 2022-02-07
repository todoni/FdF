#include "get_next_line.h"
#include "libft/libft.h"
#include "../fdf_graph.h"
#include <stdio.h>
#include <fcntl.h>

int	read_map(int fd, t_coordinate_list **save)
{
	char	*line;
	char	**words;
	int		gnl_return_val;
	int		num_points;
	int		num_per_line;
	int 	row;
	t_coordinate_list *move;

	gnl_return_val = 1;
	num_points = 0;
	num_per_line = 0;
	row = 0;
	while (gnl_return_val)
	{
		gnl_return_val = get_next_line(fd, &line);
		words = ft_split(line, ' ');

		int i = 0;
		while (words[i])
		{
			move = *save;
			if (*save)
			{
				while (move->next)
					move = move->next;
				move->next = ft_calloc(1, sizeof(t_coordinate_list *));
				move->next->x = i;
				move->next->y = row;
				move->next->z = ft_atoi(words[i]);
			}
			else
			{
				*save = ft_calloc(1, sizeof(t_coordinate_list *));
				(*save)->x = i;
				(*save)->y = row;
				(*save)->z = ft_atoi(words[i]);
			}
			//printf("%d %d %d\n", i, row, ft_atoi(words[i]));
			i++;
			num_points++;
		}
		row++;
		num_per_line = i;
	}
	return (num_per_line);
}

void	put_map(t_coordinate_list *coor_list, t_coordinate **coor)
{
	int		index;

	index = 0;
	while (coor_list)
	{
		coor[index]->x = coor_list->x;
		coor[index]->y = coor_list->y;
		coor[index]->z = coor_list->z;
		coor_list = coor_list->next;
	}
}
