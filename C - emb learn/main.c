#include <stdio.h>
#include <stdint.h>

// Маска бита по номеру
#define BIT(n) (1 << n)
// Инверсная маска бита по номеру 
#define BIT_INV(n) ~(1 << n)

/// @brief 
/// Вывод в консоль числа int в формате "DEC HEX BIN" 
/// @param n 
void printBinaryInt(int);
/// @brief 
/// Вывод в консоль числа int8 в формате "DEC HEX BIN" 
/// @param n 
void printBinaryInt8(int8_t);

// эмуляция 1KB памяти
uint32_t fake_mem[256]; 

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

typedef union item2le
{
    struct
    {
        uint8_t d, c, b, a;
    }letter;
    struct
    {
        uint16_t cd, ab;
    }word;
    uint32_t value;    
}ITEM2LE;

int main(void)
{
    /* 
        1. Дана переменная uint16_t value = 0x1234, требуется установить в ней 1 и 3 бит в единицу не изменяя состояние остальных.
    */
    {
        printf("\t1.\n");
        uint16_t value = 0x1234;
        // Вывод в консоль значения value "До" в виде "dec hex bin"
        printBinaryInt(value);
        // Включаем биты 1 и 3 
        value |= BIT(1) | BIT(3);
        // Вывод в консоль значения value "После" в виде "dec hex bin"
        printBinaryInt(value);
    }

    /*
        2. Дана переменная uint16_t value = 0x1234, требуется сбросить в ней 4 и 12 бит в ноль не изменяя состояние остальных.
    */
    {
        printf("\t2.\n");

        uint16_t value = 0x1234;

        // Вывод в консоль значения value "До" в виде "dec hex bin"
        printBinaryInt(value);
        // Выключаем биты 4 и 12 
        value &= BIT_INV(4) & BIT_INV(12);
        // Вывод в консоль значения value "После" в виде "dec hex bin"
        printBinaryInt(value);
    }

    /*
        3. Дан адрес регистра в памяти 0x08004020. Требуется записать в этот регистр число 10 как 32 битное значение.
    */
    {
        printf("\t3.\n");
        // Установка значения в регистр
        fake_mem[0x08] = 10; //*((uint32_t*)(0x08004020)) = 10;
    }
    
    /*
        4. Дан адрес регистра в памяти 0x08004010. Требуется прочитать данный регистр и вывести на экран строку «reg адрес, bit 5 = значение». 
        При этом вместо слов адрес и значение вывести адрес регистра (желательно в hex) и значение 5-го бита данного регистра (0 или 1).
    */
    {
        printf("\t4.\n");
        // Какоето значение в памяти
        fake_mem[0x04] = 32; // 5 бит в 1
        // установка указателя на регистр
        uint32_t* ptr = &fake_mem[0x04];//(uint32_t*)(0x08004010);
        // вывод адреса указателя(регистр) и 5 бит по этому адресу
        printf("reg 0x%llX, bit 5 = %d\n", (uintptr_t)ptr, (*ptr >> 5) & 1);
    }

    /*
        5. Требуется представить число 2 как 8 битное и записать в двоичном виде. 
        Затем следует его преобразовать в -2 и тоже записать в двоичном виде. 
        Как называется представление отрицательных чисел ?
        "Дополнительный код?"
    */
    {
        printf("\t5.\n");
        int8_t value = 2;
        // Вывод в консоль значения value "До" в виде "dec hex bin"
        printBinaryInt8(value);
        // Преобразование 2 в -2
        value *= -1;
        // Вывод в консоль значения value "После" в виде "dec hex bin"
        printBinaryInt8(value);

    }

    /*
        6. Дана переменная uint32_t value = 0x12345678. Требуется расписать побайтно расположение данных в ОЗУ если порядок байт little endian.
    */
    {
        printf("\t6.\n");
        uint32_t value = 0x12345678;
        // установка указателя на первый байт value
        char* ptr = (char*)&value;
        // Вывод байт по порядку в памяти
        for(int i = 0; i < sizeof(uint32_t); i++)
            printf("0x%02X ", ptr[i]);
        printf("\n");
    }

    /*
        7. Требуется объявить и сразу проинициализировать массив uint32_t array[] некоторым количеством значений. 
        Затем в цикле используя арифметику указателей каждую ячейку умножить на 2.
        Причём count должен автоматически пересчитывать количество ячеек массива.
    */
    {
        printf("\t7.\n");

        // Объявление + инициализация массива
        int array[] = {1, 2, 3, 4};
        // Указатель на первый элемент массива
        int* ptr = array;
        // Размер массива
        int count = sizeof(array) / sizeof(*array);
        int arraySize = count;

        // вывод массива в консоль "До"
        for (int i = 0; i < arraySize; i++)
            printf("%d ", array[i]);
        printf("\n");

        // Домнажаем каждый элемент массива на 2
        while(count--)
        {
            // ptr[count] *= 2;
            *(ptr + count) *= 2;
        }

        // вывод массива в консоль "После"
        for (int i = 0; i < arraySize; i++)
            printf("%d ", array[i]);
        printf("\n");
    }

    /*
        8. Имеется 4-байтовая переменная uint32_t value = 0x12345678. 
        Если представить данную переменную как набор байт abcd, необходимо написать код, реализующий следующие операции: 
        сложить байты b и d, результат вычесть из слова cd и затем записать результат разности в слово ab.
    */
    // 8.1. Через указатели
    {
        printf("\t8.1.\n");
                    //     d c b a
                    //     3 2 1 0
        uint32_t value = 0x12345678;
        
        printf("0x%02X\n", value);

        // Указатель на первый байт числа (d)
        uint8_t* ptr8 = (uint8_t*)&value;
        // Указатель на первую пару байт числа (cd)
        uint16_t* ptr16 = (uint16_t*)&value;

        // cd = ab - (b + d)
        *ptr16 = *(ptr16 + 1) - (*(ptr8 + 1) + *(ptr8 + 3));
                                   ///   d c b a
        printf("0x%02X\n", value); /// 0x123411CC
    }

    // 8.2. Через union
    {
        printf("\t8.2.\n");
                    //     d c b a
                    //     3 2 1 0
        uint32_t value = 0x12345678;
        
        printf("0x%02X\n", value);

        union item t;   // Объявление экземпляра union item
        t.u32 = value;  // Инициализация экземпляра union item через значение u32(uint32_t)

        // cd = ab - (b + d)
        t.u16[0] = t.u16[1] - (t.u8[1] + t.u8[3]);
                                   ///   d c b a
        printf("0x%02X\n", t.u32); /// 0x123411CC
    }
    
    // 8.3. uniun + struct
    {
        printf("\t8.3.\n");
                    //     d c b a
        uint32_t value = 0x12345678;
        
        printf("0x%02X\n", value);

        union item2 t;      // Объявление экземпляра union item
        t.value = value;    // Инициализация экземпляра union item через значение value(uint32_t)

        t.word.ab = t.word.cd - (t.letter.b + t.letter.d);
                                     ///   d c b a
        printf("0x%02X\n", t.value); /// 0x123411CC
    }

    // 8.4. union + struct (байты в обратном порядке)
    {
        printf("\t8.4.le\n");
                    //     a b c d
        uint32_t value = 0x12345678;
        
        printf("0x%02X\n", value);

        union item2le t;
        t.value = value;

        t.word.ab = t.word.cd - (t.letter.b + t.letter.d);
                                     ///   a b c d
        printf("0x%02X\n", t.value); /// 0x55CC5678
    }

    return 0;
}

void printBinaryInt(int n) 
{
    unsigned int num = n;
    int bits = sizeof(int) * 8; // получаем число бит в типе int

    // Вывод числа в форматах DEC и HEX
    printf("%d\t0x%02X\t0b", n, n);

    // Проходимся по битам числа в обратном порядке. проверяем состоние бита и печатаем его
    for (int i = bits - 1; i >= 0; i--)
        printf("%d", (num >> i) & 1);
    printf("\n");
}

void printBinaryInt8(int8_t n) 
{
    unsigned int num = n;
    int bits = sizeof(int8_t) * 8; // получаем число бит в типе int8_t

    printf("%d\t0x%02X\t0b", n, n);

    for (int i = bits - 1; i >= 0; i--)
        printf("%d", (num >> i) & 1);
    printf("\n");
}