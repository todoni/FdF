/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 18:52:36 by sohan             #+#    #+#             */
/*   Updated: 2021/05/24 20:14:01 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(int c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

static int	is_exceed(const char *str)
{
	int	digit;

	digit = 0;
	while (*(str + digit) && ft_isdigit(*(str + digit)))
	{
		digit++;
	}
	if (digit > 19)
		return (-1);
	if (digit == 19 && \
		str[0] == '9' && (str[1] >= '2' || str[2] >= '2' || \
		str[3] >= '3' || str[4] >= '3' || str[5] >= '7' || \
		str[6] >= '2' || str[7] >= '0' || str[8] >= '3' || \
		str[9] >= '6' || str[10] >= '8' || str[11] >= '5' || \
		str[12] >= '4' || str[13] >= '7' || str[14] >= '7' || \
		str[15] >= '5' || str[16] >= '8' || str[17] >= '0' || \
		str[18] > '7'))
		return (-1);
	return (0);
}

int			ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (is_exceed(str) == -1 && sign == -1)
		return (0);
	else if (is_exceed(str) == -1 && sign == 1)
		return (-1);
	while (ft_isdigit(*str))
	{
		result *= 10;
		result += (*str - 48) * sign;
		str++;
	}
	return (result);
}
