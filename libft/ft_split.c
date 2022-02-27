/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/10 21:13:10 by sohan             #+#    #+#             */
/*   Updated: 2021/05/24 20:15:28 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int			count_word(const char *str, char c)
{
	int	word_count;

	word_count = 0;
	while (*str)
	{
		while (*str != c && *str)
			str++;
		if (*(str - 1) != c)
		{
			word_count++;
		}
		if (*str == '\0')
			break ;
		str++;
	}
	return (word_count);
}

static int			count_word_len(const char *str, char c)
{
	int word_len;

	word_len = 0;
	while (*str != c && *str)
	{
		str++;
		word_len++;
	}
	return (word_len);
}

static const char	*put_words(char **strs, const char *str, char c, int i)
{
	int j;

	j = 0;
	while (*str != c && *str)
	{
		strs[i][j] = *str;
		j++;
		str++;
	}
	strs[i][j] = '\0';
	while (*str == c)
		str++;
	return (str);
}

static void			free_memory(char **strs, int i)
{
	int	index;

	index = 0;
	while (index <= i)
	{
		free(strs[index]);
		index++;
	}
	free(strs);
}

char				**ft_split(const char *str, char c)
{
	char	**strs;
	int		i;
	int		num_word;

	strs = 0;
	i = 0;
	while (*str == c && *str)
		str++;
	num_word = count_word(str, c);
	strs = (char **)ft_calloc(num_word + 1, sizeof(char *));
	if (strs == 0)
		return (0);
	while (i < num_word)
	{
		strs[i] = (char *)ft_calloc(count_word_len(str, c) + 1, sizeof(char));
		if (strs[i] == 0)
		{
			free_memory(strs, i);
			return (0);
		}
		str = put_words(strs, str, c, i);
		i++;
	}
	return (strs);
}
