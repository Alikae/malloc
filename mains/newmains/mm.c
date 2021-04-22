#include "includes/malloc.h"

int main()
{
    char *a = malloc(10);
    char *a2 = malloc(100);
    char *a3 = malloc(1000);
    char *a4 = malloc(10000);
    char *a5 = malloc(15000);
    show_alloc_mem();
    print_chains();
}
