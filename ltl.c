#include "malloc.h"

int main()
{
    void *a;
    for (int i = 0; i < 1000; i++)
        a = malloc(50);
}
