#include <stdio.h>
#include <stdlib.h>
#include "mystrlib2.h"
#include "stack.h"

#define modulo(A,B) (((A) % (B) + (B)) % (B))

struct calc_op
init_calc_op()
{
        struct calc_op tmp;
        for (tmp.ind = 0; tmp.ind < NUM_OPERANDS; ++tmp.ind) {
                tmp.num[tmp.ind] = 0;
        }
        tmp.ind = 0;
        tmp.ans = 0.0;
        tmp.operation = None;
        return tmp;
}

struct expression
init_expression()
{
        struct expression tmp;
        for (int i = 0; i < NUM_OPS; ++i) {
                tmp.ops[i] = init_calc_op();
        }
        tmp.ans = 0.0;
        tmp.operation = None;
        return tmp;
}

double
do_operation(double a, double b, enum operations operation)
{
        switch (operation) {
        case Add:
                return a + b;
        case Subtract:
                return a - b;
        case Multiply:
                return a * b;
        case Divide:
                return a / b;
        case Modulo:
                return (double)modulo((int)a, (int)b);
        case None:
                fprintf(stderr, "wtf how is this even possible.\n");
                exit(EXIT_FAILURE);
        }
        return 0; /* This shouldn't be reachable but it shuts up gcc. */
}

// vim: expandtab sw=8
