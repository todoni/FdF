/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 18:30:28 by sohan             #+#    #+#             */
/*   Updated: 2021/05/17 19:49:31 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	len = ft_strlen(s);
	if (c == 0)
		return ((char*)(s + ft_strlen(s)));
	while (len > 0)
	{
		if (s[len - 1] == (char)c)
			return ((char*)s + len - 1);
		len--;
	}
	return (0);
}
