#include "libft/get_next_line.h"
#include "libft/libft.h"
#include "../fdf_graph.h"
#include <stdio.h>
#include <fcntl.h>

int	read_map(int fd, t_coordinate_list **save, t_map *map)
{
	char	*line;
	char	**words;
	int		gnl_return_val;
	int		num_points;
	int		num_per_line;
	int 	row;
	int		i;
	t_coordinate_list *move;
	t_coordinate	*block;
	int	index;
	char	**tmp_color;

	gnl_return_val = 1;
	num_points = 0;
	num_per_line = 0;
	row = 0;
	index = 0;
	move = ft_calloc(1, sizeof(t_coordinate_list));
	move->block = ft_calloc(BUFFER_SIZE, sizeof(t_coordinate));
	if (move == NULL)
		return (-1);
	*save = move;
	while (gnl_return_val)
	{
		gnl_return_val = get_next_line(fd, &line);
		words = ft_split(line, ' ');

		num_per_line = i;
		i = 0;
		while (words[i])
		{
			move->block[index].x = row;
			move->block[index].y = ft_atoi(words[i]);
			move->block[index].z = i;
			tmp_color = ft_split(words[i], ',');
			move->block[index].color = ft_strdup(tmp_color[1]);
			free(tmp_color[0]);
			free(tmp_color[1]);
			free(tmp_color);
			//move->x = row;
			//move->y = ft_atoi(words[i]);
			//move->z = i;
			//move->color = ft_split(words[i], ',')[1];
			i++;
			num_points++;
			index++;
			if (num_points % BUFFER_SIZE == 0)
			{
				move->block = ft_calloc(BUFFER_SIZE, sizeof(t_coordinate));
				if (move->block == NULL)
					return (-1);
				move->next = *save;
				*save = move;
				index = 0;
			}
		}
		//move = ft_calloc(1, sizeof(t_coordinate_list));
		row++;
	}
	map->column = num_per_line;
	map->size = num_points;
	return (1);
}

int	read_map2(int fd, t_coordinate *save, t_map *map)
{
	char	*line;
	char	**words;
	int		gnl_return_val;
	int		num_points;
	int		num_per_line;
	int 	row;
	int		i;
	char	**color;
	t_coordinate_list *move;

	gnl_return_val = 1;
	num_points = 0;
	num_per_line = 0;
	row = 0;
	while (gnl_return_val)
	{
		gnl_return_val = get_next_line(fd, &line);
		words = ft_split(line, ' ');
		free(line);
		num_per_line = i;
		i = 0;
		while (words[i])
		{
			save[num_points].x = row;
			save[num_points].y = ft_atoi(words[i]);
			save[num_points].z = i;
			color = ft_split(words[i], ',');
			save[num_points].color = ft_strdup(color[1]);
			//printf("readmap : %d %d %d\n", save[i].x, save[i].y, save[i].z);
			free(color[0]);
			free(color[1]);
			free(color);
			free(words[i]);
			i++;
			num_points++;
		}
		free(words);
		row++;
	}
	map->size = num_points;
	map->column = num_per_line;
	map->row = row;
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
		index++;
	}
}
