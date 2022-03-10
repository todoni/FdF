#include "../includes/fdf.h"

void	init_bresenham_param(t_bresenham *bh, t_node *from, t_node *to)
{
	bh->x = *(from->screen_x);
	bh->y = *(from->screen_y);
	bh->x_next = *(to->screen_x);
	bh->y_next = *(to->screen_y);
	bh->delta_x = ft_abs(bh->x_next - bh->x);
	bh->delta_y = ft_abs(bh->y_next - bh->y);
	bh->distance = sqrt(bh->delta_x * bh->delta_x + bh->delta_y * bh->delta_y);
	if (bh->distance == 0)
		bh->distance = 1;
}

void	set_bresenham_param(t_bresenham *bh)
{
	bh->add_x = 1;
	bh->add_y = 1;
	bh->err = bh->delta_x / 2;
	if (bh->x >= bh->x_next)
		bh->add_x = -1;
	if (bh->y >= bh->y_next)
		bh->add_y = -1;
	if (bh->delta_x <= bh->delta_y)
		bh->err = -(bh->delta_y / 2);
}

void	set_bresenham_error(t_bresenham *bh)
{
	bh->e2 = bh->err;
	if (bh->e2 > -bh->delta_x)
	{
		bh->err -= bh->delta_y;
		bh->x += bh->add_x;
	}
	if (bh->e2 < bh->delta_y)
	{
		bh->err += bh->delta_x;
		bh->y += bh->add_y;
	}
}
