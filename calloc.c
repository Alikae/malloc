#include "malloc.h"

// DO WE NEED TO ALIGN?
void    *ft_calloc(size_t nmemb, size_t size)
{
    void    *ptr;

    if (nmemb && (size_t)-1 / nmemb <= size)
        return (set_error(M_E_ARGS_TOO_LARGE));
    size *= nmemb;
    if ((ptr = ft_malloc(size)))
        for (size_t i = 0; i < size / sizeof(void*); i++)
            ((void**)ptr)[i] = 0;
    return (ptr);
}
