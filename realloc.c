#include "malloc.h"

t_chaincell *cut_and_merge_cell(t_chain *chain, t_chaincell *cell, size_t size)
{
    void    *block;

    cell->next->size -= (size - cell->size);
    cell->size = size;
    cell->next->block = cell->block + size;
    if (!cell->next->size)
    {
        block = cell->block;
        remove_cell(chain, cell->next);
        cell = find_cell_by_block(chain, block);
    }
    return (cell);
}

t_chaincell *realloc_cell(t_chain *chain, t_chaincell *cell, size_t size)
{
    if (size <= cell->size)
        return (cell);
    if (cell->next->is_free && cell->block + cell->size == cell->next->block && cell->size + cell->next->size >= size)
        return (cut_and_merge_cell(chain, cell, size));
    return (0);
}

size_t      min(size_t a, size_t b)
{
    if (a < b)
        return (a);
    return (b);
}

void	    *ft_realloc(void *ptr, size_t size)
{
    // reread carefully
	t_chaincell	*cell;
	void		*p;
    size_t      size_to_copy;
    uint8_t     cell_found;

	malloc_data();
    if (!size)
    {
        ft_free(ptr);
        return (0);
    }
	size = align(size, MEM_ALIGNMENT);
    size_to_copy = 0;
    cell_found = 0;
	for (int zone = 0; zone < NB_ZONES; zone++)
	{
		if ((cell = find_cell_by_block(malloc_data()->mem_as_chain[zone], ptr)) && !cell->is_free)
		{
            cell_found = 1;
            size_to_copy = cell->size;
            if (malloc_params()->max_accepted_size[zone] < size)
            {
                p = ft_malloc(size);
                ft_memcpy(p, ptr, size_to_copy);
                ft_free(ptr);
                return (p);
            }
            cell = realloc_cell(malloc_data()->mem_as_chain[zone], cell, size);
			break;
		}
	}
	if (!cell || cell->is_free /* Useless is_free ? */)
	{
		p = ft_malloc(size);
		if (cell_found)
			ft_memcpy(p, ptr, min(size_to_copy, size));
		return (p);
	}
	return (cell->block);
}
