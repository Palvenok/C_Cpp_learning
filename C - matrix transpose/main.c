#include <stdio.h>

#define SIZE    4

void transpose(short[SIZE][SIZE]);
void print_matrix(const short[SIZE][SIZE]);

int main(void)
{
    short matrix[SIZE][SIZE] = {0};
    short* ptr = &matrix[0][0];
    int count = 0;
    while(count < SIZE * SIZE && scanf("%hd", ptr) == 1) {
        ptr++;
        count++;
    }
    
    transpose(matrix);
    print_matrix(matrix);

    return 0;
}

void transpose(short matrix[SIZE][SIZE])
{
    for(int x = 0; x < SIZE; x++)
    {
        for(int y = x + 1; y < SIZE; y++)
        {
            short temp = matrix[x][y];
            matrix[x][y] = matrix[y][x];
            matrix[y][x] = temp;
        }
    }
}


void print_matrix(const short matrix[SIZE][SIZE])
{
    for(int x = 0; x < SIZE; x++)
    {
        for(int y = 0; y < SIZE; y++)
        {
            printf("%d", matrix[x][y]);
            if (y < SIZE - 1)
                printf(" ");
        }
        printf("\n");
    }
}