/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 22:10:26 by sohan             #+#    #+#             */
/*   Updated: 2021/05/17 21:22:50 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_lstsize_np(t_list *lst)
{
	int	size;

	size = 0;
	while (lst->next)
	{
		size++;
		lst = lst->next;
	}
	return (size + 1);
}

void		ft_lstiter(t_list *lst, void (*f)(void*))
{
	size_t	size;
	size_t	i;

	i = 0;
	size = ft_lstsize_np(lst);
	while (i < size)
	{
		f(lst->content);
		lst = lst->next;
		i++;
	}
}
