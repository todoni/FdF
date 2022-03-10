#include "../includes/fdf.h"
#include "../includes/error_messages.h"
#include "../libft/libft.h"

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

void	get_map_color(t_list *lst, t_coordinate *coor)
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
}
