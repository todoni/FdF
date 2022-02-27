/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 17:00:04 by sohan             #+#    #+#             */
/*   Updated: 2021/05/10 17:52:00 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		find_digit(int n)
{
	int	digit;

	digit = 0;
	if (n == 0)
		digit = 1;
	while (n)
	{
		n /= 10;
		digit++;
	}
	return (digit);
}

char			*ft_itoa(int n)
{
	int		init;
	int		digit;
	char	*nbr;

	init = n;
	digit = find_digit(n);
	if (n < 0)
		digit++;
	nbr = (char*)ft_calloc(digit + 1, sizeof(char));
	if (nbr == 0)
		return (0);
	while (digit > 0)
	{
		if (init < 0)
			nbr[digit - 1] = -(n % 10) + 48;
		else
			nbr[digit - 1] = n % 10 + 48;
		n /= 10;
		digit--;
	}
	if (init < 0)
		nbr[0] = '-';
	return (nbr);
}
