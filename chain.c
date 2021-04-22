// OK!
#include "chain.h"
#include "malloc.h"

void	*alloc(size_t size)
{
	void	*mem;

// OPTIONNALY
    optiwrite(1, "\033[2J", 4, 0);
    size = align(size, malloc_params()->page_size);
	if ((mem = mmap(0, size,
				PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS,
				-1, 0))
			== MAP_FAILED)
		return (set_error(M_E_NOMEM));
	return (mem);
}

t_chain	*create_chain()
{
	t_chain	*chain;

	if (!(chain = alloc(sizeof(t_chain))))
		return (0);
	chain->index = 0;
	chain->next = 0;
	return (chain);
}

t_chaincell	*add_cell(t_chain *chain, t_chaincell *cell)
{
	while (chain->next && chain->index == MALLOC_CHAIN_SIZE)
		chain = chain->next;
	if (chain->index == MALLOC_CHAIN_SIZE)
	{
		if (!(chain->next = create_chain()))
			return (0);
		chain = chain->next;
	}
	chain->cells[chain->index].block = cell->block;
	chain->cells[chain->index].size = cell->size;
	chain->cells[chain->index].prev = cell->prev;
	chain->cells[chain->index].next = cell->next;
	if (cell->prev)
		cell->prev->next = &chain->cells[chain->index];
	else
		chain->cells[chain->index].prev = &chain->cells[chain->index];
	if (cell->next)
		cell->next->prev = &chain->cells[chain->index];
	else
		chain->cells[chain->index].next = &chain->cells[chain->index];
	chain->cells[chain->index].is_free = cell->is_free;
	chain->index++;
	return (&chain->cells[chain->index - 1]);
}

// WARNING! After calling this function, outter references to cells can be corrupted
void	remove_cell(t_chain *chain, t_chaincell *cell)
{
	if (cell->prev)
		cell->prev->next = cell->next;
	if (cell->next)
		cell->next->prev = cell->prev;
	while (chain->next && chain->next->index)
		chain = chain->next;
	chain->index--;
	if (cell == &chain->cells[chain->index])
		return ;
	cell->block = chain->cells[chain->index].block;
	cell->size = chain->cells[chain->index].size;
	cell->prev = chain->cells[chain->index].prev;
	cell->next = chain->cells[chain->index].next;
	cell->is_free = chain->cells[chain->index].is_free;
	if (cell->prev)
		cell->prev->next = cell;
	if (cell->next)
		cell->next->prev = cell;
	if (chain->index < MALLOC_CHAIN_SIZE / 2 && chain->next)
	{
// OPTIONNALY
    optiwrite(1, "\033[2J", 4, 0);
		munmap(chain->next, align(sizeof(t_chain), malloc_params()->page_size));
		chain->next = 0;
	}
}

t_chaincell	*find_cell_by_block(t_chain *chain, void *block)
{
	uint32_t	i;

	while (chain)
	{
		i = 0;
		while (i < chain->index)
		{
			if (chain->cells[i].block == block)
				return (&chain->cells[i]);
			i++;
		}
		chain = chain->next;
	}
	return 0;
}

uint8_t	remove_cell_by_block(t_chain *chain, void *block)
{
	t_chaincell	*cell;

	if ((cell = find_cell_by_block(chain, block)))
		remove_cell(chain, cell);
	return (cell ? 1 : 0);
}
