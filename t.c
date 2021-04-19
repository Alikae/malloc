int main() 
{
    char    *str = malloc(120);
    memcpy(str, "BONJOUR", 7); 
    free(str);
}
