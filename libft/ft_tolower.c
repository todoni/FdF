/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 18:23:44 by sohan             #+#    #+#             */
/*   Updated: 2021/05/17 20:10:05 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isupper(int c)
{
	if (c >= 65 && c <= 90)
		return (c);
	return (0);
}

int			ft_tolower(int c)
{
	int	result;

	result = c;
	if (ft_isupper(c) != 0)
		result += 32;
	return (result);
}
