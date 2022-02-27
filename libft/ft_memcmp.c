/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 19:42:19 by sohan             #+#    #+#             */
/*   Updated: 2021/05/16 21:15:32 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (*((unsigned char*)s1) == *((unsigned char*)s2) && i < n - 1)
	{
		s1++;
		s2++;
		i++;
	}
	return ((int)(*((unsigned char*)s1) - *((unsigned char*)s2)));
}
