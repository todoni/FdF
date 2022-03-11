/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_gradient_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 15:33:39 by sohan             #+#    #+#             */
/*   Updated: 2022/03/11 15:33:41 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"
#include "../includes/error_messages.h"
#include "../libft/libft.h"

void	init_color(t_gradient *dt)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		dt->sum[i] = 0;
		i++;
	}
	dt->from[0] = (0xFF0000 & 0b111111110000000000000000) >> 16;
	dt->from[1] = (0xFF0000 & 0b000000001111111100000000) >> 8;
	dt->from[2] = 0xFF0000 & 0b000000000000000011111111;
	dt->to[0] = (0xFF0000 & 0b111111110000000000000000) >> 16;
	dt->to[1] = (0xFF0000 & 0b000000001111111100000000) >> 8;
	dt->to[2] = 0xFF0000 & 0b000000000000000011111111;
}

void	set_pixel_color(int *color, char *rgb)
{
	int	full_color;

	full_color = ft_htoi(rgb);
	color[0] = (full_color & 0b111111110000000000000000) >> 16;
	color[1] = (full_color & 0b000000001111111100000000) >> 8;
	color[2] = full_color & 0b000000000000000011111111;
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
