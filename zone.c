// OK! Just check comments
#include "malloc.h"

/*
    A Zone is as following:
    [user_size][alignment]
     ^         ^- Necessary to be sure that two different user-zones cannot be contiguous on memory,
     |                that would fuck up the free system
     L___________ Define in malloc_params for all but large, large depend on the block itself
*/

size_t		align(size_t to_align, size_t alignment)
{
	if (to_align % alignment)
		to_align += alignment - to_align % alignment;
	return (to_align);
}

size_t		subzone_user_size(uint8_t zone, size_t size)
{
	return (malloc_params()->max_accepted_size[zone] ? malloc_params()->max_accepted_size[zone] * 100 : size);
}

static size_t		subzone_real_size(uint8_t zone, size_t size)
{
    // Here we add MEM_ALIGNMENT to prevent allocated user blocks
    // from two different potentially contiguous zones to be contiguous:
    // That would allow their merge while freeing
	return (align(subzone_user_size(zone, size) + MEM_ALIGNMENT,
			malloc_params()->page_size));
}

void    ft_memcpy(void *dest, void *src, size_t size)
{
    char    *s1;
    char    *s2;

    s1 = (char*)dest;
    s2 = (char*)src;
    while (size--)
        s1[size] = s2[size];
}

static t_chaincell *create_subzone(t_chain *chain, uint8_t zone, size_t size)
{
	t_subzone		*subzone;
	t_chaincell		zone_as_chain;
	t_chaincell		*cell;

// OPTIONNALY
    if (malloc_params()->options & MOPT_PRINT_META && zone != NB_ZONES - 1)
            optiwrite(1, "\033[2J", 4, 0);
	if (!(subzone = alloc(subzone_real_size(zone, size))))
		return (0);
	zone_as_chain.block = subzone;
	zone_as_chain.size = subzone_user_size(zone, size);
	zone_as_chain.is_free = 1;
	if (chain->index)
	{
		zone_as_chain.prev = chain->cells[0].prev;
		zone_as_chain.next = &chain->cells[0];
	}
	else
	{
		zone_as_chain.prev = 0;
		zone_as_chain.next = 0;
	}
	if (!(cell = add_cell(chain, &zone_as_chain)))
		return (0);
        // also munmap the arena?
    malloc_data()->stats.total_free_size += subzone_user_size(zone, size);
	return (find_cell_by_block(chain, subzone));
}

t_chaincell	*alloc_new_subzone(uint8_t zone, size_t size)
{
	if (!malloc_data()->mem_as_chain[zone])
		if (!(malloc_data()->mem_as_chain[zone] = create_chain()))
            return (0);
	return (create_subzone(malloc_data()->mem_as_chain[zone], zone, size));
}

void	free_subzone(void *to_free, uint8_t zone, size_t size)
{
// OPTIONNALY
    if (malloc_params()->options & MOPT_PRINT_META && zone != NB_ZONES - 1)
        optiwrite(1, "\033[2J", 4, 0);
	munmap(to_free, subzone_real_size(zone, size));
    malloc_data()->stats.total_free_size -= subzone_user_size(zone, size);
}
