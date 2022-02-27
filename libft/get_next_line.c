/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 13:43:19 by sohan             #+#    #+#             */
/*   Updated: 2021/06/17 12:31:01 by sohan            ###   ########.fr       */
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
	size_t	len_joined;
	t_list_gnl	*curr;
	char	*joined;

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
	joined = (char*)malloc((len_joined + 1) * sizeof(char));
	if (joined == 0)
	{
		*success = -1;
		return (0);
	}
	joined = concatenate_nodes(save, joined);
	return (joined);
}

t_list_gnl	*gnl_split(const char *str, char sep, ssize_t *success)
{
	t_list_gnl	*node;
	t_list_gnl	*temp_node;
	char	*temp_str;
	size_t	i;

	node = 0;
	i = 0;
	while (*str)
	{
		while (str[i] != sep && str[i])
			i++;
		if ((temp_str = ft_strndup(str, i)) == 0 || \
				(temp_node = ft_gnl_lstnew(temp_str, i, str[i])) == 0)
		{
			free_all_nodes(&node);
			*success = -1;
			return (0);
		}
		ft_gnl_lstadd_back(&node, temp_node);
		str += i;
		if (*str == sep)
			str++;
		i = 0;
	}
	return (node);
}

int		get_next_line(int fd, char **line)
{
	char			buffer[BUFFER_SIZE + 1];
	static t_list_gnl	*save[OPEN_MAX];
	ssize_t			read_val;

	read_val = 1;
	if (fd < 0 || BUFFER_SIZE <= 0 || line == 0)
		return (-1);
	while (read_val)
	{
		if ((read_val = read(fd, buffer, BUFFER_SIZE)) == -1)
			return (-1);
		buffer[read_val] = '\0';
		ft_gnl_lstadd_back(&save[fd], gnl_split(buffer, '\n', &read_val));
		if (ft_gnl_strchr(buffer, '\n') == 1 || read_val == -1)
			break ;
	}
	*line = gnl_strjoin(&save[fd], &read_val);
	if (save[fd] && save[fd]->newline == '\n' && read_val != -1)
	{
		free_one_and_next(&save[fd]);
		read_val = 1;
	}
	return (read_val);
}
