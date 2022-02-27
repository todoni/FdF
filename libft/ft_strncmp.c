/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 00:17:37 by sohan             #+#    #+#             */
/*   Updated: 2021/05/03 19:12:21 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int		difference;
	size_t	index;

	index = 0;
	difference = 0;
	if (n == 0)
	{
		return (0);
	}
	while (s1[index] == s2[index] && s1[index] != '\0' && \
												index < n - 1)
	{
		index++;
	}
	difference = (unsigned char)s1[index] - (unsigned char)s2[index];
	return (difference);
}
