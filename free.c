// OK!
#include "malloc.h"

void	free_and_merge_cell(t_chain *chain, t_chaincell *cell, uint8_t zone)
{
	void	*block;

	cell->is_free = 1;
	block = cell->block;
	if (cell->next->is_free && cell->block + cell->size == cell->next->block)
	{
		cell->size += cell->next->size;
		remove_cell(chain, cell->next);
		cell = find_cell_by_block(chain, block);
	}
	if (cell->prev->is_free && cell->prev->block + cell->prev->size == cell->block)
	{
		block = cell->prev->block;
		cell->prev->size += cell->size;
		remove_cell(chain, cell);
		cell = find_cell_by_block(chain, block);
	}
	if (cell->size == subzone_user_size(zone, cell->size))
	{
		free_subzone(cell->block, zone, cell->size);
		remove_cell(chain, cell);
	}
}

void	ft_free(void *ptr)
{
	uint8_t		zone;
	t_chaincell	*cell;
    t_chain     *chain;

	if (!ptr)
		return ;
	zone = -1;
	while (++zone < NB_ZONES)
	{
        chain = malloc_data()->mem_as_chain[zone];
		if (!chain || !chain->index)
			continue;
        if ((cell = find_cell_by_block(chain, ptr)))
        {
            if (cell->is_free)
                set_error(M_E_DOUBLE_FREE);
            else
            {
                malloc_data()->stats.total_alloc_size -= cell->size;
                malloc_data()->stats.total_free_size += cell->size;
	            free_and_merge_cell(chain, cell, zone);
            }
			return ;
        }
	}
    set_error(M_E_INVALID_FREE);
}
