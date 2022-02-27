/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 10:17:53 by sohan             #+#    #+#             */
/*   Updated: 2021/05/16 21:25:59 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t i;
	size_t j;
	size_t k;

	i = 0;
	j = 0;
	k = 0;
	if (ft_strlen(needle) == 0)
		return ((char*)haystack);
	while (haystack[i] != '\0' && i < len)
	{
		j = i;
		while (k < ft_strlen(needle) && haystack[j] != '\0' && \
				haystack[j] == needle[k] && j < len)
		{
			k++;
			j++;
		}
		if (k == ft_strlen(needle))
			return ((char*)haystack + i);
		k = 0;
		i++;
	}
	return (0);
}
