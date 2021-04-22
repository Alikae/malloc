int main()
{
    void *a = malloc(5000);
    a = realloc(a, 4000);
}
