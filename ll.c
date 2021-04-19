int main()
{
    void *b = ft_malloc(100);

    for (int i = 0; i < 50000; i++)
    {
        void *a = ft_malloc(100);
        ft_free(a);
    }
}
