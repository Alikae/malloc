#ifndef MALLOC_H
# define MALLOC_H

# include <pthread.h>

# include "specs.h"

typedef struct s_subzone
{
	uint8_t			mem;
}		t_subzone;

typedef struct  s_malloc_stats
{
    size_t  malloc_calls;
    size_t  realloc_calls;
    size_t  calloc_calls;
    size_t  free_calls;
    size_t  total_calls;
}               t_malloc_stats;

typedef struct	s_malloc
{
	t_chain		        *mem_as_chain[NB_ZONES];
	uint8_t		        init;
    pthread_mutex_t     mutex;
	t_malloc_error		error;
    t_malloc_stats      stats;
}		t_malloc;

void	        *malloc(size_t size);
void	        free(void *ptr);
void	        *realloc(void *ptr, size_t size);
void	        *calloc(size_t nmemb, size_t size);
void            show_alloc_mem();

// Algos
t_chaincell	    *first_fit(uint8_t zone, size_t size);
t_chaincell	    *first_fit_backward(uint8_t zone, size_t size);

// Debug
void            set_malloc_options(t_malloc_option opt);
void            unset_malloc_options(t_malloc_option opt);
void	        print_chains();

//
void		    *ft_malloc(size_t size);
void		    ft_free(void *ptr);
void		    *ft_realloc(void *ptr, size_t size);
void	        *ft_calloc(size_t nmemb, size_t size);
t_malloc_params *malloc_params();
t_malloc	    *malloc_data();
void		    *set_error(t_malloc_error error);
const char	    *get_error();
t_chaincell	    *alloc_new_subzone(uint8_t zone, size_t size);
void		    free_subzone(void *to_free, uint8_t zone, size_t size);
size_t		    align(size_t to_align, size_t alignment);
size_t		    subzone_user_size(uint8_t zone, size_t size);
void            ft_memcpy(void *dest, void *src, size_t size);
size_t          ft_strlen(const char *str);
void            print_size(size_t size);
void            print_address(void *address);
void            print_allocated_mem_as_strings();

#endif
