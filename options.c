// OK!
#include "malloc.h"

void    set_malloc_options(t_malloc_option opt)
{
   malloc_params()->options |= opt;
}

void    unset_malloc_options(t_malloc_option opt)
{
   malloc_params()->options &= ~opt;
}
