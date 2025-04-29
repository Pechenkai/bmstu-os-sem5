#include <stdio.h>
#include <unistd.h>

#define MAX_FIB 46
#define OK 0
#define OVERFLOW_ERR 1
#define RANGE_ERR 2
#define IO_ERR 3

/// Функция нахождения числа Фибоначчи
/// 
/// \param n [in] - переменная хранящая занчение номера фибонначи
/// \param  fib [out] - указатель на число Фибоначчи
///
/// \return Целый код возврата(ОК - 0)
int fibonacci(int n, int *fib);

int fibonacci(int n, int *fib)
{
    int a = 0, b = 1, i, curr;

    if (n == 0)
    {
        *fib = a;
        return OK;
    }
    else if (n == 1)
    {
        *fib = b;
        return OK;
    }
    else if (n > MAX_FIB)
    {
        return OVERFLOW_ERR;
    }
    else
    {
        for (i = 1; i < n; i++)
        {
            curr = a;
            a = b;
            b += curr;
        }
        *fib = b;
        return OK;
    }
}

int main(void)
{
    int n, fib = 0, f;

    printf("PID: %d, Введите число n: \n", getpid());
    if (scanf("%d", &n) != 1)
    {
        printf("PID: %d, IO_ERR\n", getpid());
        return IO_ERR;
    }

    if (n < 0)
    {
        printf("PID: %d, RANGE ERROR\n", getpid());
        return RANGE_ERR;
    }

    f = fibonacci(n, &fib);
    if (f == OVERFLOW_ERR)
    {
        printf("PID: %d, OVERFLOW_ERROR\n", getpid());
        return OVERFLOW_ERR;
    }
    else
    {
        printf("PID: %d, Число Фибоначчи n: %d\n", getpid(), fib);
        return OK;
    }
}

