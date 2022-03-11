#include "../libft/get_next_line.h"
#include "../includes/error_messages.h"
#include "../includes/fdf.h"

static int	get_width(char **block, t_map *map)
{
	int	i;

	i = 0;
	while (block[i])
		i++;
	if (map->column != i && i != 0 && map->column != 0)
		terminate(ERR_MAP_INVALID);
	if (i != 0)
		map->column = i;
	return (map->column);
}

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
		map->column = get_width(block, map);
		ft_lstadd_back(&list_block, tmp);
	}
	map->size = map->row * map->column;
	return (list_block);
}
