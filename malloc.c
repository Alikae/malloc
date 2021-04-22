// OK !
#include "malloc.h"

uint8_t		cut_cell(t_chain *chain, t_chaincell *cell, size_t size)
{
	t_chaincell	newcell;

	newcell.block = cell->block + size;
	newcell.prev = cell;
	newcell.next = cell->next;
	newcell.size = cell->size - size;
	newcell.is_free = 1;
	if (!add_cell(chain, &newcell))
        return (0);
	cell->size = size;
	return (1);
}

t_chaincell	*first_fit_backward(uint8_t zone, size_t size)
{
	t_chain		*mem_as_chain;
	t_chaincell	*cell;

	mem_as_chain = malloc_data()->mem_as_chain[zone];
	cell = &mem_as_chain->cells[0];
    // instead of search by next, search by [i] for speed and concision
    // make it another test algo
    // this one search backward to optimize huge use of malloc without free
	while (cell)
	{
		if (cell->size >= size && cell->is_free)
		{
			if (cell->size > size && !cut_cell(mem_as_chain, cell, size))
				return (0);
			cell->is_free = 0;
			return (cell);
		}
		cell = cell->prev;
		if (cell == &mem_as_chain->cells[0])
			return (0);
	}
	return (0);
}

t_chaincell	*first_fit(uint8_t zone, size_t size)
{
	t_chain		*mem_as_chain;
	t_chaincell	*cell;

	mem_as_chain = malloc_data()->mem_as_chain[zone];
	cell = &mem_as_chain->cells[0];
	while (cell)
	{
		if (cell->size >= size && cell->is_free)
		{
			if (cell->size > size && !cut_cell(mem_as_chain, cell, size))
				return (0);
			cell->is_free = 0;
			return (cell);
		}
		cell = cell->next;
		if (cell == &mem_as_chain->cells[0])
			return (0);
	}
	return (0);
}

static void *find_free_space(uint8_t zone, size_t size)
{
	t_chaincell	*cell;

	if (!malloc_data()->mem_as_chain[zone]
        || !malloc_data()->mem_as_chain[zone]->index
        || !(cell = malloc_params()->algo(zone, size)))
	{
		if (!(cell = alloc_new_subzone(zone, size)))
			return (0);
		cell->is_free = 0;
		 if (cell->size > size)
			if (!cut_cell(malloc_data()->mem_as_chain[zone], cell, size))
                return (0);
	}
	return (cell->block);
}

uint8_t	dispatch_zone(size_t size)
{
	uint8_t	zone;

	zone = -1;
	while (++zone < NB_ZONES)
		if (malloc_params()->max_accepted_size[zone] >= size)
			return (zone);
	return (NB_ZONES - 1);
}

void	*ft_malloc(size_t size)
{
	uint8_t	zone;
	void	*mem;

	size = align(size ? size : 1, MEM_ALIGNMENT);
	zone = dispatch_zone(size);
	mem = find_free_space(zone, size);
	return (mem);
}
