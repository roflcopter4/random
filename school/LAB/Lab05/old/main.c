#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "mystrlib2.h"
#include "stack.h"

#define INPUTFILE "input.txt"

static void print_results(struct expression *expr);
static struct expression parse_line(String *line);


int
main(void)
{
        StringArray data = {NULL, 0};

        if (get_all_lines(&data, INPUTFILE) != 0)
                exit(EXIT_FAILURE);

        struct expression exprlist2[data.num];
        for (int i = 0; i < data.num; ++i) {
                exprlist2[i] = parse_line(&data.arr[i]);
                print_results(&exprlist2[i]);
        }

        free_string_array(&data);
        return 0;
}


static struct expression
parse_line(String *line)
{
        int paren, opnum;
        struct expression expr = init_expression();
        paren = opnum = 0;

        for (unsigned int i = 0; i < line->len; ++i) {
#               define cur_op expr.ops[opnum]
                char ch = line->str[i];
                if (isdigit(ch)) {
                        cur_op.num[cur_op.ind++] = atoi(&ch);
                } else {
                        switch (ch) {
                        case '(':
                                ++paren;
                                break;
                        case ')':
                                --paren;
                                break;
                        case ' ':
                                break;
                        default:
                                if (paren && !(expr.ops[opnum].operation)) {
                                        cur_op.operation = ch;
                                } else if (expr.operation == None) {
                                        expr.operation = ch;
                                        ++opnum;
                                }
                        }
                }
#               undef cur_op
        }
        return expr;
}


static void
print_results(struct expression *expr)
{
        String msg = {NULL, 0};
        char buf[64];
        for (int opnum = 0; opnum < NUM_OPS; ++opnum) {
#               define cur_op expr->ops[opnum]
                if (cur_op.operation != None) {
                        sprintf(buf, "(%d %d %c) ", cur_op.num[0], cur_op.num[1],
                                                    cur_op.operation);
                        cur_op.ans = cast_and_do_op(cur_op.num[0], cur_op.num[1],
                                                    cur_op.operation);
                        my_string_app(&msg, buf);
                } else {
                        sprintf(buf, "%d ", cur_op.num[0]);
                        cur_op.ans = (double)cur_op.num[0];
                        my_string_app(&msg, buf);
                }
#               undef cur_op
        }
        expr->ans = do_operation(expr->ops[0].ans, expr->ops[1].ans, expr->operation);
        my_string_app(&msg, (char *) & (expr->operation));
        printf("%-17s  = %f\n", msg.str, expr->ans);
        free(msg.str);
}


// vim: sw=8
