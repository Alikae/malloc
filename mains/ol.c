#include "malloc.h"

int main()
{
	int i = 0;
	void	*t[5000];

	for (i = 0; i < 50; i++)
		t[i] = malloc(10);
	for (i = 0; i < 50; i++)
		free(t[i]);
		print_chains();
}
