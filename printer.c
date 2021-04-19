#include "malloc.h"

void    ft_sleep()
{
    size_t n = 150000000;
    if (malloc_params()->options & MOPT_SLEEP)
        while (n--);
}

void    optiflush(int fd, char buffer[4096], size_t *size)
{
    write(fd, buffer, *size);
    *size = 0;
}

void    optiwrite(int fd, const char *s, size_t size, uint8_t flush)
{
    static char     buffer[8192];
    static int      i = 0;

    while (size--)
    {
        buffer[i++] = *(s++);
        if (i > 8100)
            optiflush(fd, buffer, &i);
    }
    if (flush)
        optiflush(fd, buffer, &i);
}

void	print_chains()
{
	int	i;

    optiwrite(1, "\033[2J", 4, 0);
	optiwrite(1, "Chains except BIG:\n", 19, 0);
	i = 0;
	while (i < NB_ZONES - 0)
	{
		optiwrite(1, malloc_params()->name[i], ft_strlen(malloc_params()->name[i]), 0);
		optiwrite(1, ":\n", 2, 0);
		print_chain(malloc_data()->mem_as_chain[i]);
		i++;
	}
    optiwrite(1, 0, 0, 1);
    ft_sleep();
}

void	print_cell(t_chaincell *cell)
{
	uint32_t	i;

	optiwrite(1, cell->is_free ? "\33[35m" : "\33[32m", 5, 0);
	optiwrite(1, "L", 1, 0);
	i = 1;
	while (i++ < cell->size / MEM_ALIGNMENT)
	{
		optiwrite(1, cell->is_free ? "\33[46;1m " : "\33[42;1m ", 8, 0);
	}
	optiwrite(1, "\33[0m", 4, 0);
}

void	print_cell_data(t_chaincell *cell)
{
    print_address(cell);
	//printf("%p", cell);
	optiwrite(1, cell->is_free ? "\33[46;1m" : "\33[42;1m", 7, 0);
    print_address(cell);
    print_address(cell->block);
    optiwrite(1, " | ", 3, 0);
    print_size(cell->size);
    optiwrite(1, " | ", 3, 0);
    print_address(cell->prev);
    optiwrite(1, " | ", 3, 0);
    print_address(cell->next);
	optiwrite(1, "\33[0m", 4, 0);
}

void	print_single_chain_data(t_chain *chain)
{
	uint32_t	i;
	t_chaincell	*cell;

	if (!chain->index)
		return ;
	optiwrite(1, "[", 1, 0);
	cell = &chain->cells[0];
	i = 0;
	while (++i)
	{
		print_cell_data(cell);
		if (cell->next == &chain->cells[0])
			break;
		optiwrite(1, ", ", 2, 0);
		if (cell->block + cell->size != cell->next->block)
			optiwrite(1, "\n*********************************\n", 35, 0);
		else if (!(i % 4))
			optiwrite(1, "\n", 1, 0);
		cell = cell->next;
	}
	optiwrite(1, "]", 1, 0);
}

void	print_single_chain(t_chain *chain)
{
	uint32_t	i;
	t_chaincell	*cell;

    print_size(chain->index);
    optiwrite(1, "\n", 1, 0);
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
			optiwrite(1, "\n*********************************\n", 35, 0);
		cell = cell->next;
	}
}

void	print_chain(t_chain *chain)
{
	optiwrite(1, "[", 1, 0);
	if (chain)
		print_single_chain(chain);
	optiwrite(1, "\33[0m] \n", 7, 0);
}

void	print_chain_data(t_chain *chain)
{
	while (chain)
	{
		print_single_chain_data(chain);
		chain = chain->next;
		if(chain)
			optiwrite(1, "\n->", 3, 0);
	}
	optiwrite(1, "\n", 1, 0);
}

void    print_allocated_mem_as_strings()
{
    t_chain         *chain;
    t_chaincell     *cell;

	optiwrite(1, "AS STRINGS [\33[35m\n", 18, 0);
    for (int zone = 0; zone < NB_ZONES; zone++)
    {
        chain = malloc_data()->mem_as_chain[zone];
        if (chain && chain->index)
        {
            cell = &chain->cells[0];
            do {
                if (!cell->is_free)
                {
                    optiwrite(1, cell->block, cell->size, 0);
	                optiwrite(1, "\n", 1, 0);
                }
                cell = cell->next;
            } while (cell != &chain->cells[0]);
        }
    }
    optiwrite(1, "\33[0m]\n", 7, 1);
    ft_sleep();
}

uint8_t is_printable(char c)
{
    return (c == 10 || (c > 31 && c < 127));
}

void    print_allocated_mem_only_strings()
{
    t_chain         *chain;
    t_chaincell     *cell;

	optiwrite(1, "AS STRINGS [\33[35m\n", 18, 0);
    for (int zone = 0; zone < NB_ZONES; zone++)
    {
        chain = malloc_data()->mem_as_chain[zone];
        if (chain && chain->index)
        {
            cell = &chain->cells[0];
            do {
                if (!cell->is_free)
                {
                    if (is_printable(*(char*)(cell->block))
                        && is_printable(*((char*)(cell->block) + 1)))
                    {
                        optiwrite(1, cell->block, cell->size, 0);
	                    optiwrite(1, "\n", 1, 0);
                    }
                }
                cell = cell->next;
            } while (cell != &chain->cells[0]);
        }
    }
    optiwrite(1, "\33[0m]\n", 7, 1);
}

t_chaincell *get_lowest_address_from_zone(size_t zone, t_chaincell *lowest)
{
    t_chaincell *cell;
    t_chaincell *first_cell;
    t_chaincell *minimum;

    optiwrite(1, "gaf ", 4, 0);
    print_address(lowest? lowest->block: 0);
    first_cell = &malloc_data()->mem_as_chain[zone]->cells[0];
    cell = first_cell;
    minimum = 0;
    do {
        if(!(lowest && lowest->block >= cell->block) && (!minimum || cell->block < minimum->block))
            minimum = cell;
        cell = cell->next;
    } while (cell != first_cell);
    optiwrite(1, "\nret ", 5, 0);
    print_address(minimum ? minimum->block : 0);
    optiwrite(1, "\n", 1, 0);
    return (minimum);
}

void    print_number_recursively(size_t n, const char *abecedary, size_t base)
{
    if (n)
    {
        print_number_recursively(n / base, abecedary, base);
        optiwrite(1, abecedary + n % base, 1, 0);
    }
}

void    print_size(size_t size)
{
    if (size)
        print_number_recursively(size, "0123456789", 10);
    else
        optiwrite(1, "0", 1, 0);
}

void    print_address(void *address)
{
    optiwrite(1, "0x", 2, 0);
    print_number_recursively((size_t)address, "0123456789ABCDEF", 16);
}

void    print_alloc_cell(t_chaincell *cell)
{
    print_address(cell->block);
    optiwrite(1, " - ", 3, 0);
    print_address(cell->block + cell->size);
    optiwrite(1, " : ", 3, 0);
    print_number_recursively(cell->size, "0123456789", 10);
    optiwrite(1, "o\n", 2, 0);
}

void    show_alloc_mem()
{
    const char  *name;
    t_chaincell *cell;

    for (uint8_t zone = 0; zone < NB_ZONES; zone++)
    {
        if (!malloc_data()->mem_as_chain[zone]
            || !malloc_data()->mem_as_chain[zone]->index)
            continue;
        name = malloc_params()->name[zone];
        optiwrite(1, name, ft_strlen(name), 0);
        optiwrite(1, ":\n", 2, 0);
        cell = 0;
        while ((cell = get_lowest_address_from_zone(zone, cell)))
        {
            print_address(cell->block);
            optiwrite(1, "\n FROM \n", 8, 0);
            print_address(cell);
            optiwrite(1, "\n", 1, 0);
            print_address(cell->next);
            optiwrite(1, "\n", 1, 0);
            do {
                if (!cell->is_free)
                    print_alloc_cell(cell);
                cell = cell->next;
            } while (cell->prev->block + cell->prev->size == cell->block);
            cell = cell->prev;
        }
    }
    optiwrite(1, 0, 0, 1);
    ft_sleep();
}
