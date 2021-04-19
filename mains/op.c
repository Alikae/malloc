#include "malloc.h"

int main()
{
	void	*p = ft_malloc(30);
	void	*q = ft_malloc(30);
	ft_free(p);
	p = ft_malloc(36);
	ft_free(p);
	p = ft_malloc(36);
	ft_free(p);
	ft_free(q);
	q = ft_malloc(36);
	p = ft_malloc(36);
	ft_free(q);
	ft_free(p);
	print_chains();
}
