#include <stdio.h>
#include <string.h>

int filter_latin(const char c) 
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) 
    {
        return 0;
    }
    return 1;
}

void copy_string(char* dst, size_t max_len_dst, const char* src, int (*filter)(char)) 
{
    size_t i = 0;
    size_t j = 0;
    
    while (src[i] != '\0' && j < max_len_dst - 1) {
        if (filter(src[i])) {
            dst[j] = src[i];
            j++;
        }
        i++;
    }
    dst[j] = '\0';
}

int main(void) 
{
    char str[100] = {0}, str2[20];
    fgets(str, sizeof(str)-1, stdin);
    char* ptr_n = strrchr(str, '\n');
    if (ptr_n != NULL)
        *ptr_n = '\0';

    copy_string(str2, sizeof(str2), str, filter_latin);

    puts(str2);

    return 0;
}