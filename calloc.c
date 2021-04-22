// OK!

#include "malloc.h"

void    *ft_calloc(size_t nmemb, size_t size)
{
    void    *ptr;
    size_t  i;

	size = align(size ? size : 1, MEM_ALIGNMENT);
    if ((size_t)-1 / size <= nmemb)
        return (set_error(M_E_ARGS_TOO_LARGE));
    size *= nmemb;
    if ((ptr = ft_malloc(size)))
    {
        i = -1;
        while (++i < size / sizeof(void*))
            ((void**)ptr)[i] = 0;
    }
    return (ptr);
}
