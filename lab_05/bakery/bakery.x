struct REQUEST
{
    int number;
    char operation;
    double arg1;
    double arg2;
};

program BAKERY_PROG
{
    version BAKERY_VER
    {
        int GET_NUMBER(void) = 1;
        double BAKERY_SERVICE(struct REQUEST) = 2;
    } = 1; /* Version number = 1 */
} = 0x20000001; /* RPC program number */
2