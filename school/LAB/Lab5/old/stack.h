#ifndef STACK_H
#define STACK_H

#define NUM_OPERANDS 2
#define NUM_OPS 2

enum operations
{
        None     = 0,
        Add      = '+',
        Subtract = '-',
        Multiply = '*',
        Divide   = '/',
        Modulo   = '%'
};

struct calc_op
{
        int num[NUM_OPERANDS];
        enum operations operation;
        int ind;
        double ans;
};

struct expression
{
        struct calc_op ops[NUM_OPS];
        enum operations operation;
        double ans;
};


struct calc_op init_calc_op();
struct expression init_expression();

double do_operation(double a, double b, enum operations operation);
#define cast_and_do_op(A,B,OP) do_operation((double)(A), (double)(B), OP)

#endif /* End stack.h */

// vim: sts=0 sw=0 noexpandtab nolist
