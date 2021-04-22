int main()
{
    char* a = malloc(100);
    char* a2 = malloc(100);
    char* a3 = malloc(100);
    free(a2);
    char* a4 = malloc(50);
}
