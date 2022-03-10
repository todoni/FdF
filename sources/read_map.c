#include "../libft/get_next_line.h"
#include "../includes/error_messages.h"
#include "../includes/fdf.h"

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
		if (*line)
			map->row++;
		block = ft_split(line, ' ');
		free(line);
		tmp = ft_lstnew(block);
		if (!block || !tmp || gnl_return_val == -1)
			terminate(ERR_MAP_READING);
		ft_lstadd_back(&list_block, tmp);
	}
	return (list_block);
}
