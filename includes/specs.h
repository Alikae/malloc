#ifndef SPECS_H
# define SPECS_H

# include "chain.h"

# define NB_ZONES       3
# define MEM_ALIGNMENT  16

typedef enum    e_malloc_option
{
    MOPT_PRINT_ERROR    = 1,
    MOPT_PRINT_META     = 2,
    MOPT_PRINT_ALL      = 4,
    MOPT_PRINT_AS_STRINGS       = 8,
    MOPT_PRINT_ALLOC_MEM        = 16,
    MOPT_SLEEP          = 32,
    MOPT_PRINT_NO_LARGE          = 64,
    MOPT_PRINT_ONLY_TINY          = 128,
    MOPT_PRINT_STEP          = 256,
}               t_malloc_option;

typedef enum    e_malloc_error
{
    M_E_NOMEM,
    M_E_INVALID_FREE,
    M_E_DOUBLE_FREE,
    M_E_ARGS_TOO_LARGE,
}               t_malloc_error;

typedef struct	s_malloc_params
{
	const char	*name[NB_ZONES];
	unsigned int	max_accepted_size[NB_ZONES];
	t_chaincell	*(*algo)(uint8_t, size_t);
    int32_t     options;
    uint32_t    print_nth_steps;
	size_t		page_size;
}		t_malloc_params;

#endif
