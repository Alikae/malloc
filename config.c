// OK!
#include "malloc.h"

/*
    Don't forget to update NB_ZONES in malloc.h if you change it here
ALGOS:
    first_fit
    first_fit_backward
OPTIONS:
    MOPT_PRINT_ERROR
    | MOPT_PRINT_AS_STRINGS
    | MOPT_PRINT_META
    | MOPT_PRINT_ALL
    | MOPT_PRINT_ALLOC_MEM
    | MOPT_SLEEP
    | MOPT_PRINT_NO_LARGE
OLD SIZES:
    {16, 256, 0},
*/

static void set_env_options()
{
    int32_t options;

    options = 0;
    if (getenv("MOPT_PRINT_AS_STRINGS"))
        options |= MOPT_PRINT_AS_STRINGS;
    if (getenv("MOPT_PRINT_META"))
        options |= MOPT_PRINT_META;
    if (getenv("MOPT_PRINT_ALL"))
        options |= MOPT_PRINT_ALL;
    if (getenv("MOPT_PRINT_ALLOC_MEM"))
        options |= MOPT_PRINT_ALLOC_MEM;
    if (getenv("MOPT_SLEEP"))
        options |= MOPT_SLEEP;
    if (getenv("MOPT_PRINT_NO_LARGE"))
        options |= MOPT_PRINT_NO_LARGE;
    if (getenv("MOPT_PRINT_ONLY_TINY"))
        options |= MOPT_PRINT_ONLY_TINY;
    if (getenv("MOPT_PRINT_STEP"))
        options |= MOPT_PRINT_STEP;
    if (getenv("MOPT_ADAPT_STATS"))
        options |= MOPT_ADAPT_STATS;
    set_malloc_options(options);
}

t_malloc_params	*malloc_params()
{
	static t_malloc_params	params = {
		{"SMALL", "MEDIUM", "LARGE"},
		{256, 2048, 0},
		first_fit_backward,
        MOPT_PRINT_ERROR
        ,
        1000,
		0
	};
	return (&params);
}

t_malloc	*malloc_data()
{
	static t_malloc	malloc = {0};

	if (!malloc.init)
	{
		malloc_params()->page_size = getpagesize();
		malloc.init = 1;
        pthread_mutex_init(&malloc.mutex, 0);
        set_env_options();
	}
	return (&malloc);
}
