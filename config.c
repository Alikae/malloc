#include "malloc.h"

// Don't forget to update NB_ZONES in malloc.h if you change it here
t_malloc_params	*malloc_params()
{
	static t_malloc_params	params = {
		{"SMALL", "MEDIUM", "LARGE"},
		{16, 256, 0},
		first_fit,
        MOPT_PRINT_ERROR
                //| MOPT_PRINT_AS_STRINGS
                //| MOPT_PRINT_META
                //| MOPT_PRINT_ALL
                //| MOPT_PRINT_ALLOC_MEM
                //| MOPT_SLEEP
        ,
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
        pthread_mutex_init(&malloc.mutex, 0); // check params
	}
	return (&malloc);
}
