#include "../includes/fdf.h"
#include "../includes/error_messages.h"
#include "../libft/libft.h"

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

void	get_delta_color(t_gradient *dt)
{
	dt->delta[0] = dt->to[0] - dt->from[0];
	dt->delta[1] = dt->to[1] - dt->from[1];
	dt->delta[2] = dt->to[2] - dt->from[2];
}

void	set_color_gradient(t_gradient *dt)
{
	dt->sum[0] += dt->delta[0];
	dt->sum[1] += dt->delta[1];
	dt->sum[2] += dt->delta[2];
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

void	set_pixel_color(int *color, char *rgb)
{
	int	full_color;

	full_color = ft_htoi(rgb);
	color[0] = (full_color & 0b111111110000000000000000) >> 16;
	color[1] = (full_color & 0b000000001111111100000000) >> 8;
	color[2] = full_color & 0b000000000000000011111111;
}
