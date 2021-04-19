#include "malloc.h"

int main() {
    void*   m0 = ft_malloc(40);
    void*   mA = ft_malloc(400);
    m0 = ft_realloc(m0, 10);
    m0 = ft_realloc(m0, 100);
    m0 = ft_realloc(m0, 1000);
    m0 = ft_realloc(m0, 10);
    ft_free(m0);
    m0 = ft_realloc(m0, 10000);
    m0 = ft_realloc(m0, 0);
    ft_free(mA);
    print_chains();
}
