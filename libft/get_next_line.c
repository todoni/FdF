/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:21:16 by sohan             #+#    #+#             */
/*   Updated: 2022/03/09 18:21:28 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif

static void	move_str_addr(const char **str, char sep, int i)
{
	*str += i;
	if (**str == sep)
		(*str)++;
}

t_list_gnl	*gnl_split(const char *str, char sep, ssize_t *success)
{
	t_list_gnl	*node;
	t_list_gnl	*temp_node;
	char		*temp_str;
	size_t		i;

	node = 0;
	i = 0;
	while (*str)
	{
		while (str[i] != sep && str[i])
			i++;
		temp_str = ft_strndup(str, i);
		temp_node = ft_gnl_lstnew(temp_str, i, str[i]);
		if (temp_str == 0 || temp_node == 0)
		{
			free_all_nodes(&node);
			*success = -1;
			return (0);
		}
		ft_gnl_lstadd_back(&node, temp_node);
		move_str_addr(&str, sep, i);
		i = 0;
	}
	return (node);
}

int	get_next_line(int fd, char **line)
{
	char				buffer[BUFFER_SIZE + 1];
	static t_list_gnl	*save[OPEN_MAX];
	ssize_t				read_val;

	read_val = 1;
	if (fd < 0 || BUFFER_SIZE <= 0 || line == 0)
		return (-1);
	while (read_val)
	{
		read_val = read(fd, buffer, BUFFER_SIZE);
		if (read_val == -1)
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
