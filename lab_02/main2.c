#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <unistd.h>

#define OK 0
#define IO_ERR 1
#define RANGE_ERR 2

/// @brief Функция печати числа в двоичном формате
/// @param a переменная содержащая число
/// @param nbits переменная содержащая число бит в числе
void bin_print(uint32_t a, size_t nbits);

/// @brief Функция циклического битового сдвига числа
/// @param a переменная содержащая число
/// @param n переменная содержащая количество битов для сдвига
/// @param nbits переменная содержащая число бит в числе
/// @return целое беззнаковое значение числа псле битового сдвига
uint32_t bit_shifting(uint32_t a, int n, size_t nbits);

void bin_print(uint32_t a, size_t nbits)
{
    for (int i = nbits; i > 0; --i)
        printf("%d", (a & (1U << (i - 1))) ? 1 : 0);
    printf("\n");
}

uint32_t bit_shifting(uint32_t a, int n, size_t nbits)
{
    uint32_t mask_1 = 1U << (nbits - 1);
    uint32_t mask_2 = 1;
    for (int i = 1; i <= n; i++)
    {
        if (a & mask_1)
            a = (a << 1) | mask_2;
        else
            a <<= 1;
        //a = (a << 1) | (a >> (nbits - 1));
    }
    return a;
}

int main(void)
{
    uint32_t a;
    int n;
    size_t nbits = sizeof (a) * CHAR_BIT;

    printf("PID: %d, Введите числа a, n: \n", getpid());
    if (scanf("%u %d", &a, &n) != 2)
    {
        printf("PID: %d, Error: invalid number of arguments passed.\n", getpid());
        return IO_ERR;
    }

    if (n < 0)
    {
        printf("PID: %d, Error: input value of parameter n is negative.\n", getpid());
        return RANGE_ERR;
    }

    //a = (a << n) | (a >> (nbits - n));
    a = bit_shifting(a, n, nbits);
    printf("PID: %d, Результат: ", getpid());
    bin_print(a, nbits);
    return OK;
}

