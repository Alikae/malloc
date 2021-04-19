#include "malloc.h"

static void    print_data(const char *name, size_t size)
{
    if (malloc_params()->options & MOPT_PRINT_ALL)
        write(1, name, size);
    if (malloc_params()->options & MOPT_PRINT_AS_STRINGS)
        print_allocated_mem_as_strings();
    if (malloc_params()->options & MOPT_PRINT_ALLOC_MEM)
        show_alloc_mem();
    if (malloc_params()->options & MOPT_PRINT_META)
        print_chains();
}

void	*malloc(size_t size)
{
    void    *ptr;

    pthread_mutex_lock(&malloc_data()->mutex);
    print_data("MALLOC:\n", 8);
	ptr = ft_malloc(size);
    pthread_mutex_unlock(&malloc_data()->mutex);
    return (ptr);
}

void	free(void *ptr)
{
    pthread_mutex_lock(&malloc_data()->mutex);
    print_data("FREE:\n", 6);
	ft_free(ptr);
    pthread_mutex_unlock(&malloc_data()->mutex);
}

void	*realloc(void *ptr, size_t size)
{
    void    *mem;

    pthread_mutex_lock(&malloc_data()->mutex);
    print_data("REALLOC:\n", 9);
	mem = ft_realloc(ptr, size);
    pthread_mutex_unlock(&malloc_data()->mutex);
    return (mem);
}

void	*calloc(size_t nmemb, size_t size)
{
    void    *mem;

    pthread_mutex_lock(&malloc_data()->mutex);
    print_data("CALLOC:\n", 8);
	mem = ft_calloc(nmemb, size);
    pthread_mutex_unlock(&malloc_data()->mutex);
    return (mem);
}
