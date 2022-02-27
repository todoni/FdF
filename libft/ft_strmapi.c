/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 18:24:31 by sohan             #+#    #+#             */
/*   Updated: 2021/05/10 18:59:17 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	index;
	char			*s_mapped;

	index = 0;
	s_mapped = (char*)ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (s_mapped == 0)
		return (0);
	while (index < ft_strlen(s))
	{
		s_mapped[index] = f(index, s[index]);
		index++;
	}
	return (s_mapped);
}
