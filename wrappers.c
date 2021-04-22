// OK!
#include "malloc.h"

static void    print_data(const char *name, size_t size)
{
    if (malloc_params()->options & MOPT_PRINT_STEP && malloc_data()->stats.total_calls % malloc_params()->print_nth_steps)
        return;
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
    malloc_data()->stats.malloc_calls += 1;
    malloc_data()->stats.total_calls += 1;
    print_data("MALLOC:\n", 8);
    pthread_mutex_unlock(&malloc_data()->mutex);
    return (ptr);
}

void	free(void *ptr)
{
    pthread_mutex_lock(&malloc_data()->mutex);
	ft_free(ptr);
    malloc_data()->stats.free_calls += 1;
    malloc_data()->stats.total_calls += 1;
    print_data("FREE:\n", 6);
    pthread_mutex_unlock(&malloc_data()->mutex);
}

void	*realloc(void *ptr, size_t size)
{
    pthread_mutex_lock(&malloc_data()->mutex);
	ptr = ft_realloc(ptr, size);
    malloc_data()->stats.realloc_calls += 1;
    malloc_data()->stats.total_calls += 1;
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
    malloc_data()->stats.calloc_calls += 1;
    malloc_data()->stats.total_calls += 1;
    pthread_mutex_unlock(&malloc_data()->mutex);
    return (ptr);
}
