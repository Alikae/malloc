#include "malloc.h"
#include "errno.h"

size_t      ft_strlen(const char *str)
{
    size_t  i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

void		*set_error(t_malloc_error error)
{
    const char  *err;

	if (error == M_E_NOMEM || error == M_E_ARGS_TOO_LARGE)
        errno = ENOMEM;
	malloc_data()->error = error;
    if (malloc_params()->options & MOPT_PRINT_ERROR)
    {
        err = get_error();
        write(1, "MALLOC_ERROR: ", 14);
        write(1, err, ft_strlen(err));
        write(1, "\n", 1);
    }
    return (0);
}

const char	*get_error()
{
	t_malloc_error  error;

	error = malloc_data()->error;
	if (error == M_E_NOMEM)
		return ("M_E_NOMEM");
	else if (error == M_E_INVALID_FREE)
		return ("M_E_INVALID_FREE");
	else if (error == M_E_DOUBLE_FREE)
		return ("M_E_INVALID_FREE (potential DOUBLE_FREE)");
	else if (error == M_E_ARGS_TOO_LARGE)
		return ("M_E_ARGS_TOO_LARGE (type overflow)");
	return ("Undefined Error");
}
