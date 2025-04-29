/*
 * filename: calculator.x
     * function: Define constants, non-standard data types and the calling process in remote calls
 */

const ADD = 0;
const SUB = 1;
const MUL = 2;
const DIV = 3;

struct CALCULATOR
{
    int op;
    double arg1;
    double arg2;
    double result;
};

program CALCULATOR_PROG
{
    version CALCULATOR_VER
    {
        struct CALCULATOR CALCULATOR_PROC(struct CALCULATOR) = 1;
    } = 1; /* Version number = 1 */
} = 0x20000001; /* RPC program number */
