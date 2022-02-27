/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 19:55:47 by sohan             #+#    #+#             */
/*   Updated: 2021/05/10 20:16:12 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static void	display_nbr(int n, int fd)
{
	char	c;

	if (n == 0)
		return ;
	display_nbr(n / 10, fd);
	if (n < 0)
		c = -(n % 10) + 48;
	else
		c = n % 10 + 48;
	write(fd, &c, 1);
}

void		ft_putnbr_fd(int n, int fd)
{
	if (n < 0)
		write(fd, "-", 1);
	if (n == 0)
	{
		write(fd, "0", 1);
		return ;
	}
	display_nbr(n, fd);
}
