#ifndef STACK_H
#define STACK_H

#define NUM_OPERANDS 2
#define NUM_OPS 32

enum operators {
        None     = 0,
        Add      = '+',
        Subtract = '-',
        Multiply = '*',
        Divide   = '/',
        Modulo   = '%'
};


union operand {
        int num;
        struct calc_op *sub_op;
};


struct calc_op {
        int num[NUM_OPERANDS];
        /*union operand opnd[NUM_OPERANDS];*/
        enum operators operator;
        int ind;
        double ans;
};


struct expression {
        struct calc_op ops[NUM_OPS];
        enum operators operator[NUM_OPS];
        double ans;
        int numops;
};


struct calc_op init_calc_op();
struct expression init_expression();

double do_operation(enum operators operator, double a, double b);
#define cast_and_do_op(OP,A,B) do_operation((OP), (double)(A), (double)(B))

#endif /* End stack.h */
