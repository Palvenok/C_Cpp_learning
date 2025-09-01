#include <stdio.h>
#include <stdint.h>

void printBinaryInt(int);
void printBinaryInt8(int8_t);

uint32_t fake_mem[256]; // эмуляция 1KB памяти

typedef union item
{
    uint8_t u8[4];
    uint16_t u16[2];
    uint32_t u32;
}ITEM;

typedef union item2
{
    struct
    {
        uint8_t a, b, c, d;
    }letter;
    struct
    {
        uint16_t ab, cd;
    }word;
    uint32_t value;    
}ITEM2;

int main(void)
{
    // 1. =================================
    {
        printf("\t1.\n");
        uint16_t value = 0x1234;

        printBinaryInt(value);
        value |= 10;
        printBinaryInt(value);
    }

    // 2. =================================
    {
        printf("\t2.\n");

        uint16_t value = 0x1234;

        printBinaryInt(value);
        value &= ~4112;
        printBinaryInt(value);
    }

    // 3. =================================
    {
        printf("\t3.\n");

        fake_mem[0x08] = 10;//*((uint32_t*)(0x08004020)) = 10;
    }
    
    // 4. =================================
    {
        printf("\t4.\n");
        fake_mem[0x04] = 32;
        uint32_t* ptr = &fake_mem[0x04];//(uint32_t*)(0x08004010);
        printf("reg 0x%llX, bit 5 = %d\n", (uintptr_t)ptr, (*ptr >> 5) & 1);
    }

    // 5. =================================
    {
        printf("\t5.\n");
        int8_t value = 2;
        printBinaryInt8(value);
        value *= -1;
        printBinaryInt8(value);

    }

    // 6. =================================
    {
        printf("\t6.\n");
        uint32_t value = 0x12345678;
        char* ptr = (char*)&value;
        for(int i = sizeof(uint32_t) - 1; i >= 0; i--)
            printf("0x%02X ", ptr[i]);
        printf("\n");
    }

    // 7. =================================
    {
        printf("\t7.\n");

        int array[] = {1, 2, 3, 4};
        int* ptr = array;
        int count = sizeof(array) / sizeof(*array);
        
        for (int i = 0; i < sizeof(array) / sizeof(*array); i++)
            printf("%d ", ptr[i]);
        printf("\n");

        while(count--)
        {
            // ptr[count] *= 2;
            *(ptr + count) *= 2;
        }

        for (int i = 0; i < sizeof(array) / sizeof(*array); i++)
            printf("%d ", ptr[i]);
        printf("\n");
    }

    // 8.1. =================================
    {
        printf("\t8.1.\n");
                    //     a b c d
                    //     3 2 1 0
        uint32_t value = 0x12345678;
        
        printf("0x%02X\n", value);

        uint8_t* ptr8 = (uint8_t*)&value;
        uint16_t* ptr16 = (uint16_t*)&value;

        *(ptr16 + 1) = *ptr16 - (*ptr8 + *(ptr8 + 2));
                                   ///   a b c d
        printf("0x%02X\n", value); /// 0x55CC5678
    }

    // 8.2. =================================
    {
        printf("\t8.2.\n");
                    //     a b c d
                    //     3 2 1 0
        uint32_t value = 0x12345678;
        
        printf("0x%02X\n", value);

        union item t;
        t.u32 = value;

        t.u16[1] = t.u16[0] - (t.u8[0] + t.u8[2]);
                                   ///   a b c d
        printf("0x%02X\n", t.u32); /// 0x55CC5678
    }

    // 8.3. =================================
    {
        printf("\t8.3.\n");
                    //     a b c d
        uint32_t value = 0x12345678;
        
        printf("0x%02X\n", value);

        union item2 t;
        t.value = value;

        t.word.cd = t.word.ab - (t.letter.a + t.letter.c);
                                     ///   a b c d
        printf("0x%02X\n", t.value); /// 0x55CC5678
    }

    return 0;
}

void printBinaryInt(int n) 
{
    unsigned int num = n; 
    int i;
    int bits = sizeof(int) * 8;

    printf("%d\t0x%02X\t0b", n, n);

    for (i = bits - 1; i >= 0; i--) 
    {
        if ((num >> i) & 1) 
        {
            printf("1");
        } 
        else 
        {
            printf("0");
        }
    }
    printf("\n");
}

void printBinaryInt8(int8_t n) 
{
    unsigned int num = n; 
    int i;
    int bits = sizeof(int8_t) * 8;

    printf("%d\t0x%02X\t0b", n, n);

    for (i = bits - 1; i >= 0; i--) 
    {
        if ((num >> i) & 1) 
        {
            printf("1");
        } 
        else 
        {
            printf("0");
        }
    }
    printf("\n");
}