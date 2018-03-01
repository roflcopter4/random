#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mystrlib2.h"
#include "stack.h"

/*#define INPUTFILE "input.txt"*/
#define INPUTFILE "NOPE.txt"

static struct expression parse_line(String *line);
static void _Assign_Operator_(struct expression *expr, int ch, int paren);
static void _Assign_Buffer_(struct expression *expr, char *buf, int *buf_index, int *token);
static void print_results(struct expression *expr);

#define assign_operator(CH) _Assign_Operator_(&expr, CH, paren)
#define assign_buffer() _Assign_Buffer_(&expr, buf, &buf_index, &token)


int main(int argc, char **argv)
{
        int i;
        char *filename;
        StringArray data = {NULL, 0};

        /* WARNING HORRIFIC HACK FOLLOWS KEEP AWAY */
        if (argc > 1) {
                int ret = 0;
                for (i = 1; i < argc; ++i) {
                        if (i > 1)
                                putchar('\n');
                        ret += main(0, &argv[i]); 
                }
                return ret;
        }
        if (argc == 0)
                filename = argv[0];
        else
                filename = INPUTFILE;

        /* Get info from file/stdin */
        if (strcmp(filename, "-") == 0) {
                printf("Processing input from stdin.\n");
                get_stdin(&data);
        } else {
                printf("Processing input file '%s'.\n", filename);
                if (get_all_lines(&data, filename) != 0)
                        return 1;
        }

        /* Actual work. */
        struct expression exprlist[data.num];  /* The ONE feature in C that's not in C++. */

        for (i = 0; i < data.num; ++i) {
                exprlist[i] = parse_line(&data.arr[i]);
                print_results(&exprlist[i]);
        }

        SA_free(&data);
        return 0;
}


static struct expression
parse_line(String *line)
{
        int paren, dash, token, buf_index;
        unsigned int i;
        char buf[line->len];
        struct expression expr = init_expression();

        paren = dash = token = buf_index = 0;
        buf[0] = '\0';  /* ensure we don't get garbage numbers from the stack */
        #define cur_op expr.ops[expr.numops]

        for (i = 0; i < line->len; ++i) {
                int ch = line->str[i];
                #define prev_ch line->str[i-1]

                if (isdigit(ch)) {
                        if (dash) {
                                buf[buf_index++] = '-';
                                --dash;
                        }
                        buf[buf_index++] = ch;
                        buf[buf_index] = '\0';
                        ++token;
                } else {
                        switch (ch) {
                        case '(':
                                if (dash) {
                                        assign_operator('-');
                                        --dash;
                                }
                                ++paren;
                                break;

                        case ')':
                                --paren;
                        case ' ':
                                if (dash) {
                                        assign_operator('-');
                                        --dash;
                                } else if (token) {  /* end of token */
                                        assign_buffer();
                                }
                                break;

                        case '-':
                                if (dash) {
                                        assign_operator('-');
                                } else if (token) {
                                        assign_buffer();
                                        assign_operator('-');
                                } else {
                                        ++dash;
                                }

                        case '\n':
                        case '\r':
                        case '\0':
                        case EOF:
                                break;

                        default:
                                if (token)
                                        assign_buffer();
                                assign_operator(ch);
                        }
                }
        }

        if (token)
                cur_op.num[cur_op.ind++] = atoi(buf);

        return expr;
}
#undef cur_op


#define cur_op expr->ops[expr->numops]

static void
_Assign_Operator_(struct expression *expr, int ch, int paren)
{
        if (ch == Add || ch == Subtract || ch == Multiply || ch == Divide || ch == Modulo) {
                if (paren && (cur_op.operator == None)) {
                        cur_op.operator = ch;
                } else {
                        expr->operator[expr->numops++] = ch;
                }
        }
}


static void
_Assign_Buffer_(struct expression *expr, char *buf, int *buf_index, int *token)
{
        cur_op.num[cur_op.ind++] = atoi(buf);
        *token = *buf_index = 0;
        buf[0] = '\0';
}

#undef cur_op


static void
print_results(struct expression *expr)
{
        String msg = {NULL, 0};
        char buf[256];
        #define cur_op expr->ops[i]
        #define prev_op expr->ops[i-1]

        for (int i = 0; i <= expr->numops; ++i) {
                if (cur_op.operator != None) {
                        sprintf(buf, "(%d %d %c) ", cur_op.num[0], cur_op.num[1],
                                cur_op.operator);
                        cur_op.ans = cast_and_do_op(cur_op.operator,
                                                    cur_op.num[0], cur_op.num[1]);
                        my_string_app(&msg, buf);
                } else {
                        sprintf(buf, "%d ", cur_op.num[0]);
                        cur_op.ans = (double)cur_op.num[0];
                        my_string_app(&msg, buf);
                }
                if (i > 0) {
                        if (expr->operator[i] != None) {
                                sprintf(buf, "%c ", expr->operator[i-1]);
                                my_string_app(&msg, buf);
                        } else {
                                my_string_app(&msg, (char *)&(expr->operator[i-1]));
                        }

                        if (i == 1)
                                expr->ans = do_operation(expr->operator[i-1], prev_op.ans, cur_op.ans);
                        else if (i > 1)
                                expr->ans = do_operation(expr->operator[i-1], expr->ans, cur_op.ans);
                }
        }

        printf("%9.2f =  %s\n", expr->ans, msg.str);
        saferFree(msg.str);
}
#undef cur_op
#undef prev_op
