/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 18:27:45 by sohan             #+#    #+#             */
/*   Updated: 2021/05/17 19:47:44 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	if (c == 0)
		return ((char*)(s + ft_strlen(s)));
	while (*s)
	{
		if (*s == (char)c)
			return ((char*)s);
		s++;
	}
	return (0);
}
