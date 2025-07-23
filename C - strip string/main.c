#include <stdio.h>
#include <string.h>

void strip_str(char *str, const char *filtr) 
{
    char *src = str;
    char *dst = str;

    while (*src) 
    {
        if (!strchr(filtr, *src)) 
            *dst++ = *src;
        src++;
    }
    *dst = '\0';
}

int main(void)
{
    char str[100] = {0}, str2[20];
    fgets(str, sizeof(str)-1, stdin);
    char* ptr_n = strrchr(str, '\n');
    if(ptr_n != NULL)
        *ptr_n = '\0';

    strip_str(str, ".,-!?");
    puts(str);

    return 0;
}