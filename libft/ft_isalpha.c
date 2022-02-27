/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 18:16:21 by sohan             #+#    #+#             */
/*   Updated: 2021/05/17 20:04:46 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isupper(int c)
{
	if (c >= 65 && c <= 90)
		return (c);
	return (0);
}

static int	ft_islower(int c)
{
	if (c >= 97 && c <= 122)
		return (c);
	return (0);
}

int			ft_isalpha(int c)
{
	if (ft_isupper(c) != 0 || ft_islower(c) != 0)
		return (c);
	return (0);
}
