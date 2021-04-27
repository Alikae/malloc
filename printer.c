// Last TODO...
#include "malloc.h"

void    ft_sleep()
{
    size_t n = 150000000;
    if (malloc_params()->options & MOPT_SLEEP)
        while (n--);
}

void    optiflush(char buffer[4096], size_t *size)
{
    write(malloc_params()->mallog_fd, buffer, *size);
    *size = 0;
}

void    optiwrite(const char *s, size_t size, uint8_t flush)
{
    // BUFFER OVERFLOW when printing "as string" a large block (4096+...)
    static char     buffer[8192];
    static size_t   i = 0;

    if (size > 1000)
    {
        optiflush(buffer, &i);
        write(malloc_params()->mallog_fd, s, size);
        return ;
    }
    while (size--)
        buffer[i++] = *(s++);
    if (i > 7000 || flush)
        optiflush(buffer, &i);
}

void    count_tiny_stats(size_t *alloc_size, size_t *free_size)
{
    t_chain     *mem_as_chain;
    t_chaincell *cell;

    *alloc_size = 0;
    *free_size = 0;
    if (!(mem_as_chain = malloc_data()->mem_as_chain[0]))
        return ;
	cell = &mem_as_chain->cells[0];
	while (cell)
	{
        if (cell->is_free)
            *free_size += cell->size;
        else
            *alloc_size += cell->size;
		cell = cell->next;
		if (cell == &mem_as_chain->cells[0])
			return ;
	}
}

void    subtract_large_stats(size_t *alloc_size)
{
    t_chain     *mem_as_chain;
    t_chaincell *cell;

    if (!(mem_as_chain = malloc_data()->mem_as_chain[NB_ZONES - 1]))
        return;
	cell = &mem_as_chain->cells[0];
	while (cell)
	{
        *alloc_size -= cell->size;
		cell = cell->next;
		if (cell == &mem_as_chain->cells[0])
			return ;
	}
}

void    print_malloc_stats()
{
    optiwrite("\33[0;0H", 6, 0);
	optiwrite("mallocs: ", 9, 0);
    print_size(malloc_data()->stats.malloc_calls);
	optiwrite(", callocs: ", 11, 0);
    print_size(malloc_data()->stats.calloc_calls);
	optiwrite(", reallocs: ", 12, 0);
    print_size(malloc_data()->stats.realloc_calls);
	optiwrite(", frees: ", 9, 0);
    print_size(malloc_data()->stats.free_calls);
//	optiwrite(", Total: ", 9, 0);
//    print_size(malloc_data()->stats.total_calls);
	optiwrite(".\n", 2, 0);
    size_t alloc_size;
    size_t free_size;
    alloc_size = malloc_data()->stats.total_alloc_size;
    free_size = malloc_data()->stats.total_free_size;
    if (malloc_params()->options & MOPT_ADAPT_STATS)
    {
        if (malloc_params()->options & MOPT_PRINT_NO_LARGE)
        {
            subtract_large_stats(&alloc_size);
        }
        else if (malloc_params()->options & MOPT_PRINT_ONLY_TINY)
        {
            count_tiny_stats(&alloc_size, &free_size);
        }
    }
	optiwrite("total alloced size: ", 20, 0);
    print_size(alloc_size / 1000);
	optiwrite("kB, total free size: ", 21, 0);
    print_size(free_size / 1000);
	optiwrite("kB, used mem: ", 14, 0);
    print_size((uint8_t)(((double)alloc_size / (double)(free_size + alloc_size)) * 100.f));
	optiwrite("%.\n", 3, 1);
}

void	print_chains()
{
	int	i;
    uint8_t zone_max;

    //optiwrite("\033[2J", 4, 0);
    print_malloc_stats();
    if (malloc_params()->options & MOPT_PRINT_ONLY_TINY)
	    optiwrite("Chains SMALL:\n", 14, 0);
    else if (malloc_params()->options & MOPT_PRINT_NO_LARGE)
	    optiwrite("Chains except BIG:\n", 19, 0);
    else
	    optiwrite("Chains:\n", 8, 0);
	i = 0;
    if (!(malloc_params()->options & MOPT_PRINT_ONLY_TINY))
        zone_max = NB_ZONES - (malloc_params()->options & MOPT_PRINT_NO_LARGE ? 1 : 0);
    else
        zone_max = 1;
	while (i < zone_max)
	{
		optiwrite(malloc_params()->name[i], ft_strlen(malloc_params()->name[i]), 0);
		optiwrite(":\n", 2, 0);
		print_chain(malloc_data()->mem_as_chain[i]);
		i++;
	}
    optiwrite(0, 0, 1);
    ft_sleep();
}

void	print_cell(t_chaincell *cell)
{
	uint32_t	i;

	optiwrite(cell->is_free ? "\33[35m" : "\33[32m", 5, 0);
	optiwrite("L", 1, 0);
	i = 1;
	while (i++ < cell->size / MEM_ALIGNMENT)
	{
		optiwrite(cell->is_free ? "\33[46;1m " : "\33[42;1m ", 8, 0);
	}
	optiwrite("\33[0m", 4, 0);
}

void	print_cell_data(t_chaincell *cell)
{
    print_address(cell);
	optiwrite(cell->is_free ? "\33[46;1m" : "\33[42;1m", 7, 0);
    print_address(cell);
    print_address(cell->block);
    optiwrite(" | ", 3, 0);
    print_size(cell->size);
    optiwrite(" | ", 3, 0);
    print_address(cell->prev);
    optiwrite(" | ", 3, 0);
    print_address(cell->next);
	optiwrite("\33[0m", 4, 0);
}

void	print_single_chain_data(t_chain *chain)
{
	uint32_t	i;
	t_chaincell	*cell;

	if (!chain->index)
		return ;
	optiwrite("[", 1, 0);
	cell = &chain->cells[0];
	i = 0;
	while (++i)
	{
		print_cell_data(cell);
		if (cell->next == &chain->cells[0])
			break;
		optiwrite(", ", 2, 0);
		if (cell->block + cell->size != cell->next->block)
			optiwrite("\n*********************************\n", 35, 0);
		else if (!(i % 4))
			optiwrite("\n", 1, 0);
		cell = cell->next;
	}
	optiwrite("]", 1, 0);
}

void	print_single_chain(t_chain *chain)
{
	uint32_t	i;
	t_chaincell	*cell;

	if (!chain->index)
		return ;
	cell = &chain->cells[0];
	i = 0;
	while (++i)
	{
		print_cell(cell);
		if (cell->next == &chain->cells[0])
			break;
		if (cell->block + cell->size != cell->next->block)
			optiwrite("\n*********************************\n", 35, 0);
		cell = cell->next;
	}
}

void	print_chain(t_chain *chain)
{
	optiwrite("[", 1, 0);
	if (chain)
		print_single_chain(chain);
	optiwrite("\33[0m] \n", 7, 0);
}

void	print_chain_data(t_chain *chain)
{
	while (chain)
	{
		print_single_chain_data(chain);
		chain = chain->next;
		if(chain)
			optiwrite("\n->", 3, 0);
	}
	optiwrite("\n", 1, 0);
}

void    print_allocated_mem_as_strings()
{
    t_chain         *chain;
    t_chaincell     *cell;

	optiwrite("AS STRINGS [\33[35m\n", 18, 0);
    for (int zone = 0; zone < NB_ZONES; zone++)
    {
        chain = malloc_data()->mem_as_chain[zone];
        if (chain && chain->index)
        {
            cell = &chain->cells[0];
            do {
                if (!cell->is_free)
                {
                    optiwrite(cell->block, cell->size, 0);
	                optiwrite("\n", 1, 0);
                }
                cell = cell->next;
            } while (cell != &chain->cells[0]);
        }
    }
    optiwrite("\33[0m]\n", 6, 1);
    ft_sleep();
}

uint8_t is_printable(char c)
{
    return ((c > 31 && c < 127) || c == 10 || c == 9);
}

size_t is_printable_string(char *s, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (s[i] == 0)
            return (i);
        if (!is_printable(s[i]))
            return (i);
    }
    return (size);
}

void    print_allocated_mem_only_strings()
{
    t_chain         *chain;
    t_chaincell     *cell;
    size_t          len;

	optiwrite("AS STRINGS [\33[35m\n", 18, 0);
    for (int zone = 0; zone < NB_ZONES; zone++)
    {
        chain = malloc_data()->mem_as_chain[zone];
        if (chain && chain->index)
        {
            cell = &chain->cells[0];
            do {
                if (!cell->is_free)
                {
                    if (cell->size < 1024 && (len = is_printable_string((char*)cell->block, cell->size)))
                    {
                        optiwrite(cell->block, len, 0);
	                    optiwrite("\n", 1, 0);
                    }
                }
                cell = cell->next;
            } while (cell != &chain->cells[0]);
        }
    }
    optiwrite("\33[0m]\n", 7, 1);
}

t_chaincell *get_lowest_address_from_zone(size_t zone, t_chaincell *lowest)
{
    t_chaincell *cell;
    t_chaincell *first_cell;
    t_chaincell *minimum;

    first_cell = &malloc_data()->mem_as_chain[zone]->cells[0];
    cell = first_cell;
    minimum = 0;
    do {
        if(!(lowest && lowest->block >= cell->block) && (!minimum || cell->block < minimum->block))
            minimum = cell;
        cell = cell->next;
    } while (cell != first_cell);
    return (minimum);
}

void    print_number_recursively(size_t n, const char *abecedary, size_t base)
{
    if (n)
    {
        print_number_recursively(n / base, abecedary, base);
        optiwrite(abecedary + n % base, 1, 0);
    }
}

void    print_size(size_t size)
{
    if (size)
        print_number_recursively(size, "0123456789", 10);
    else
        optiwrite("0", 1, 0);
}

void    print_address(void *address)
{
    optiwrite("0x", 2, 0);
    print_number_recursively((size_t)address, "0123456789ABCDEF", 16);
}

void    print_alloc_cell(t_chaincell *cell)
{
    print_address(cell->block);
    optiwrite(" - ", 3, 0);
    print_address(cell->block + cell->size);
    optiwrite(" : ", 3, 0);
    print_number_recursively(cell->size, "0123456789", 10);
    optiwrite("o\n", 2, 0);
}

void    show_alloc_mem_intern()
{
    const char  *name;
    t_chaincell *cell;

    for (uint8_t zone = 0; zone < NB_ZONES; zone++)
    {
        if (!malloc_data()->mem_as_chain[zone]
            || !malloc_data()->mem_as_chain[zone]->index)
            continue;
        name = malloc_params()->name[zone];
        optiwrite(name, ft_strlen(name), 0);
        optiwrite(":\n", 2, 0);
        cell = 0;
        while ((cell = get_lowest_address_from_zone(zone, cell)))
        {
            do {
                if (!cell->is_free)
                    print_alloc_cell(cell);
                cell = cell->next;
            } while (cell->prev->block + cell->prev->size == cell->block);
            cell = cell->prev;
        }
    }
    optiwrite(0, 0, 1);
    ft_sleep();
}

void    show_alloc_mem()
{
    const char  *name;
    t_chaincell *cell;

    pthread_mutex_lock(&malloc_data()->mutex);
    for (uint8_t zone = 0; zone < NB_ZONES; zone++)
    {
        if (!malloc_data()->mem_as_chain[zone]
            || !malloc_data()->mem_as_chain[zone]->index)
            continue;
        name = malloc_params()->name[zone];
        optiwrite(name, ft_strlen(name), 0);
        optiwrite(":\n", 2, 0);
        cell = 0;
        while ((cell = get_lowest_address_from_zone(zone, cell)))
        {
            do {
                if (!cell->is_free)
                    print_alloc_cell(cell);
                cell = cell->next;
            } while (cell->prev->block + cell->prev->size == cell->block);
            cell = cell->prev;
        }
    }
    optiwrite(0, 0, 1);
    pthread_mutex_unlock(&malloc_data()->mutex);
}
