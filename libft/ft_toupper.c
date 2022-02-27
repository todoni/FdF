/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 18:26:47 by sohan             #+#    #+#             */
/*   Updated: 2021/05/17 20:09:12 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_islower(int c)
{
	if (c >= 97 && c <= 122)
		return (c);
	return (0);
}

int			ft_toupper(int c)
{
	int	result;

	result = c;
	if (ft_islower(c) != 0)
		result -= 32;
	return (result);
}
