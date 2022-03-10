#include "libft.h"

static int	find_index(char c)
{
	char	*radix;
	int		index;

	index = 0;
	radix = "0123456789abcdef";
	c = ft_tolower(c);
	while (index < 16)
	{
		if (radix[index] == c)
			return (index);
		index++;
	}
	return (0);
}

int	ft_htoi(const char *str)
{
	int	result;

	result = 0;
	str += 2;
	while (ft_isdigit(*str) || ('a' <= *str && *str <= 'f') \
										|| ('A' <= *str && *str <= 'F'))
	{
		result *= 16;
		result += find_index(*str);
		str++;
	}
	return (result);
}
