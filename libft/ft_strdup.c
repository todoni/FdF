/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 19:36:02 by sohan             #+#    #+#             */
/*   Updated: 2021/05/04 12:59:50 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*ptr_copy;

	len = 0;
	len = ft_strlen(s1);
	ptr_copy = (char *)malloc(len * sizeof(char) + 1);
	if (ptr_copy == 0)
		return (0);
	while (*s1)
	{
		*ptr_copy = *s1;
		s1++;
		ptr_copy++;
	}
	*ptr_copy = '\0';
	ptr_copy -= len;
	return (ptr_copy);
}
