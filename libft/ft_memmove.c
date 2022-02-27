/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 19:02:57 by sohan             #+#    #+#             */
/*   Updated: 2021/05/16 21:10:32 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	if (dst > src)
	{
		while (len >= 1)
		{
			*((unsigned char*)dst + len - 1) = *((unsigned char*)src + len - 1);
			len--;
		}
	}
	else if (dst < src)
		dst = ft_memcpy(dst, (void*)src, len);
	return (dst);
}
