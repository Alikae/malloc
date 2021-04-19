#ifndef CHAIN_H
# define CHAIN_H

# include <stdint.h>
# include <unistd.h>
# include <sys/mman.h>
//tmp
# include <stdio.h>

// A simulation of double linked list in a single memory block.
// The chain can itself be chained.

# define MALLOC_CHAIN_SIZE	250

typedef struct	s_chaincell
{
	void			    *block;
	struct s_chaincell	*prev;
	struct s_chaincell	*next;
	uint32_t		    size;
	uint8_t			    is_free;
}		        t_chaincell;

typedef struct	s_chain
{
	uint32_t        index;
	struct s_chain  *next;
	t_chaincell     cells[MALLOC_CHAIN_SIZE];
}		        t_chain;

void		*alloc(size_t size);
t_chain		*create_chain();
void		destroy_chain(t_chain *chain);
t_chaincell	*add_cell(t_chain *chain, t_chaincell *cell);
void		remove_cell(t_chain *chain, t_chaincell *cell);
t_chaincell	*find_cell_by_block(t_chain *chain, void *block);
uint8_t		remove_cell_by_block(t_chain *chain, void *block);
void		print_chain(t_chain *chain);

#endif
