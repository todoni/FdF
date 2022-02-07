#include "fdf_graph.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*memory;

	memory = malloc(count * size);
	if (memory == 0)
		return (0);
	return (memory = ft_memset(memory, 0, count * size));
}
