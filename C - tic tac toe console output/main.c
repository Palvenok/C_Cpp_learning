#include <stdio.h>

#define SIZE    3

void print_array(const char[SIZE][SIZE]);

int main(void)
{
    char pole[SIZE][SIZE] = {0};
    char* ptr_p = &pole[0][0];
    int count = 0;
    int val = 0;

    while(count < SIZE * SIZE && scanf("%d", &val) == 1) {
        *ptr_p = val;
        ptr_p++;
        count++;
    }

    print_array(pole);

    return 0;
}


void print_array(const char arr[SIZE][SIZE])
{
    for(int x = 0; x < SIZE; x++)
    {
        for(int y = 0; y < SIZE; y++)
        {
            printf("%c", arr[x][y] == 1 ? 'x' : arr[x][y] == 2 ? 'o' : '#');
            if (y < SIZE - 1)
                printf(" ");
        }
        printf("\n");
    }
}