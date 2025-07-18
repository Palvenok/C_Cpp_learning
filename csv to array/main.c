#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_data_csv(double*, int, const char*);

int main(void)
{
    char str[100] = {0};
    fgets(str, sizeof(str)-1, stdin);
    char* ptr_n = strrchr(str, '\n');
    if(ptr_n != NULL)
        *ptr_n = '\0';

    // здесь продолжайте функцию main
    double arr[20] = {};

    int count = get_data_csv(arr, 20, str);

    for(int i = 0; i < count; i++)
        printf("%.2lf ", arr[i]);

    return 0;
}

int get_data_csv(double* arr, int len, const char* str)
{
    str += 5;
    char temp_str[50] = {};
    int count = 0;

    while(count < len)
    {
        char* ptr = strchr(str, ';');
        if(ptr == NULL) break;
        strncpy(temp_str, str, ptr - str);
        temp_str[ptr - str] = '\0';
        arr[count] = atof(temp_str);
        count++;
        str = ptr + 1;
    }

    char* ptr = strchr(str, '\0');
    strncpy(temp_str, str, ptr - str);
    temp_str[ptr - str] = '\0';
    arr[count] = atof(temp_str);
    count++;

    return count;
}