/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 15:37:22 by sohan             #+#    #+#             */
/*   Updated: 2022/03/11 15:37:23 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	free_one_and_next(t_list_gnl **lst)
{
	t_list_gnl	*temp;

	temp = *lst;
	*lst = (*lst)->next;
	free(temp->content);
	temp->content = 0;
	free(temp);
	temp = 0;
}

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	i;
	char	*ptr_copy;

	i = 0;
	ptr_copy = (char *)malloc((n + 1) * sizeof(char));
	if (ptr_copy == 0)
		return (0);
	while (s1[i] && i < n)
	{
		ptr_copy[i] = s1[i];
		i++;
	}
	ptr_copy[i] = '\0';
	return (ptr_copy);
}

char	*gnl_strjoin(t_list_gnl **save, ssize_t *success)
{
	size_t		len_joined;
	t_list_gnl	*curr;
	char		*joined;

	if (*success == -1)
		return (0);
	len_joined = 0;
	curr = *save;
	while (curr && curr->newline != '\n')
	{
		len_joined += curr->len;
		curr = curr->next;
	}
	if (curr)
		len_joined += curr->len;
	joined = (char *)malloc((len_joined + 1) * sizeof(char));
	if (joined == 0)
	{
		*success = -1;
		return (0);
	}
	joined = concatenate_nodes(save, joined);
	return (joined);
}
