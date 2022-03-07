#include "libft/get_next_line.h"
#include "libft.h"
#include "fdf_graph.h"
#include <stdio.h>
#include <fcntl.h>
#include "error_messages.h"

t_list	*read_map(int fd, t_map *map)
{
	char	*line;
	char	**block;
	int		gnl_return_val;
	t_list	*list_block;
	t_list	*tmp;

	gnl_return_val = 1;
	list_block = 0;
	while (gnl_return_val)
	{
		gnl_return_val = get_next_line(fd, &line);
		block = ft_split(line, ' ');
		if (*line)
			map->row++;
		free(line);
		tmp = ft_lstnew(block);
		if (!block || !tmp || gnl_return_val == -1)
			terminate(ERR_MAP_READING);
		ft_lstadd_back(&list_block, tmp);
	}
	return (list_block);
}

int	get_map_size(t_list *lst, t_map *map)
{
	int		i;
	char 	**block;

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
	char **block;

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
	while(lst)
	{
		block = lst->content;
		i = 0;
		while(block[i])
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

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*extract_color(char *str)
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

int	get_map_color(t_list *lst, t_coordinate *coor)
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
		free_array(block);
	}
	return (0);
}
