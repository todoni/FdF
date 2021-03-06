/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 10:44:50 by sohan             #+#    #+#             */
/*   Updated: 2021/06/17 12:29:10 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef OPEN_MAX
#  define OPEN_MAX 256
# endif

typedef struct s_list_gnl
{
	void				*content;
	char				newline;
	size_t				len;
	struct s_list_gnl	*next;
}	t_list_gnl;

int				get_next_line(int fd, char **line);
void			free_one_and_next(t_list_gnl **lst);
void			free_all_nodes(t_list_gnl **lst);
char			*ft_strndup(const char *s1, size_t n);
t_list_gnl		*gnl_split(const char *str, char sep, ssize_t *success);
char			*gnl_strjoin(t_list_gnl **save, ssize_t *success);
char			*concatenate_nodes(t_list_gnl **save, char *joined);
void			ft_gnl_lstadd_back(t_list_gnl **lst, t_list_gnl *new);
t_list_gnl		*ft_gnl_lstnew(void *content, size_t len, char newline);
int				ft_gnl_strchr(const char *s, int c);

#endif
