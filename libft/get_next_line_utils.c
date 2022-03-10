/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 10:43:48 by sohan             #+#    #+#             */
/*   Updated: 2021/06/17 12:24:16 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_gnl_lstadd_back(t_list_gnl **lst, t_list_gnl *new)
{
	t_list_gnl	*move;

	move = *lst;
	if (*lst)
	{
		while (move->next)
			move = move->next;
		move->next = new;
	}
	else
		*lst = new;
}

t_list_gnl	*ft_gnl_lstnew(void *content, size_t len, char newline)
{
	t_list_gnl	*lstnew;

	lstnew = (t_list_gnl *)malloc(1 * sizeof(t_list_gnl));
	if (lstnew == 0)
		return (0);
	lstnew->content = content;
	lstnew->len = len;
	lstnew->newline = newline;
	lstnew->next = 0;
	return (lstnew);
}

void	free_all_nodes(t_list_gnl **lst)
{
	while (*lst)
		free_one_and_next(lst);
}

int	ft_gnl_strchr(const char *s, int c)
{
	if (c == 0)
		return (0);
	while (*s)
	{
		if (*s == (char)c)
			return (1);
		s++;
	}
	return (0);
}

char	*concatenate_nodes(t_list_gnl **save, char *joined)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (*save && (*save)->newline != '\n')
	{
		while (j < (*save)->len)
		{
			*(joined + i) = *((char *)((*save)->content) + j);
			i++;
			j++;
		}
		free_one_and_next(save);
		j = 0;
	}
	while (*save && j < (*save)->len)
	{
		*(joined + i) = *((char *)((*save)->content) + j);
		i++;
		j++;
	}
	joined[i] = '\0';
	return (joined);
}
