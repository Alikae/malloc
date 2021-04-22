int main()
{
    char buf[1];
    void *a = malloc(200);
    printf("%p\n", a);
    read(0, buf, 1);
    free(a);
}
