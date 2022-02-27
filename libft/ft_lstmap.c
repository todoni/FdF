/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 22:36:06 by sohan             #+#    #+#             */
/*   Updated: 2021/05/17 21:17:30 by sohan            ###   ########.fr       */
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

t_list		*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
	t_list	*lst_mapped;
	t_list	*temp;
	int		size;
	int		i;

	lst_mapped = 0;
	i = 0;
	size = ft_lstsize_np(lst);
	while (i < size)
	{
		temp = ft_lstnew(f(lst->content));
		if (temp == 0)
		{
			ft_lstclear(&lst_mapped, del);
			return (lst_mapped);
		}
		ft_lstadd_back(&lst_mapped, temp);
		lst = lst->next;
		i++;
	}
	return (lst_mapped);
}
