// OK!
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
	ptr = ft_malloc(size);
    print_data("MALLOC:\n", 8);
    pthread_mutex_unlock(&malloc_data()->mutex);
    return (ptr);
}

void	free(void *ptr)
{
    pthread_mutex_lock(&malloc_data()->mutex);
	ft_free(ptr);
    print_data("FREE:\n", 6);
    pthread_mutex_unlock(&malloc_data()->mutex);
}

void	*realloc(void *ptr, size_t size)
{
    pthread_mutex_lock(&malloc_data()->mutex);
	ptr = ft_realloc(ptr, size);
    print_data("REALLOC:\n", 9);
    pthread_mutex_unlock(&malloc_data()->mutex);
    return (ptr);
}

void	*calloc(size_t nmemb, size_t size)
{
    void    *ptr;

    pthread_mutex_lock(&malloc_data()->mutex);
	ptr = ft_calloc(nmemb, size);
    print_data("CALLOC:\n", 8);
    pthread_mutex_unlock(&malloc_data()->mutex);
    return (ptr);
}
