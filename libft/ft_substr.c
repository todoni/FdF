/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 15:24:45 by sohan             #+#    #+#             */
/*   Updated: 2021/05/17 19:35:34 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;

	i = 0;
	if (len + start > ft_strlen(s))
		len = ft_strlen(s + start);
	substr = (char*)ft_calloc(len + 1, sizeof(char));
	if (substr == 0)
		return (0);
	while (start < ft_strlen(s) && i < len)
	{
		if (*(s + start + i) == '\0')
			break ;
		*(substr + i) = *(s + start + i);
		i++;
	}
	return (substr);
}
