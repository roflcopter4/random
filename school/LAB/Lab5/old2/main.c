#include "mystrlib2.h"
#include "stack.h"
#include <ctype.h>
/*#include <stdio.h>*/
#include <stdlib.h>

#define INPUTFILE "input.txt"

static void print_results(struct expression *expr, unsigned int maxlen);
static struct expression parse_line(String *line);


int main(void)
{
        unsigned int maxlen = 0;
        StringArray data = {NULL, 0};

        if (get_all_lines(&data, INPUTFILE) != 0)
                exit(EXIT_FAILURE);

        struct expression exprlist[data.num];

        for (int i = 0; i < data.num; ++i) {
                maxlen = (data.arr[i].len > maxlen) ? data.arr[i].len : maxlen;
                exprlist[i] = parse_line(&data.arr[i]);
        }

        for (int i = 0; i < data.num; ++i)
                print_results(&exprlist[i], maxlen);

        free_string_array(&data);
        return 0;
}


#define assign_operator(CH) do                         \
{                                                      \
        if (paren && (cur_op.operator == None))        \
                cur_op.operator = (CH);                \
        else                                           \
                expr.operator[expr.numops++] = (CH);   \
} while (0)


static struct expression
parse_line(String *line)
{
        int paren, dash, token, buf_index;
        char buf[line->len];
        struct expression expr = init_expression();

        paren = dash = token = buf_index = 0;
        buf[0] = '\0';  /* ensure we don't get garbage numbers from the stack */
        #define cur_op expr.ops[expr.numops]

        for (unsigned int i = 0; i < line->len; ++i) {
                int ch = line->str[i];
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
                                ++paren;
                                break;
                        case ')':  /* Both space and ')' mark end of token. */
                                --paren;
                        case ' ':
                                if (dash) {
                                        assign_operator('-');
                                        --dash;
                                } else if (token) {  /* end of token */
                                        cur_op.num[cur_op.ind++] = atoi(buf);
                                        token = buf_index = 0;
                                }
                                break;
                        case '-':  /* Could be either subtraction or negation */
                                ++dash;
                        case '\n':
                        case '\r':
                        case '\0':
                        case EOF:
                                break;
                        default:
                                assign_operator(ch);
                        }
                }
        }

        if (token)
                cur_op.num[cur_op.ind++] = atoi(buf);

        return expr;
}
#undef cur_op


static void
print_results(struct expression *expr, unsigned int maxlen)
{
        String msg = {NULL, 0};
        char buf[64];
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
                        /*if (expr->operator[i-1] != expr->operator[i]) {*/
                                if (expr->operator[i] != None) {
                                        sprintf(buf, "%c ", expr->operator[i-1]);
                                        my_string_app(&msg, buf);
                                } else {
                                        my_string_app(&msg, (char *)&(expr->operator[i-1]));
                                }
                        /*}*/

                        if (i == 1)
                                expr->ans = do_operation(expr->operator[i-1], prev_op.ans, cur_op.ans);
                        else if (i > 1)
                                expr->ans = do_operation(expr->operator[i-1], expr->ans, cur_op.ans);
                }
        }

        printf("%-*s = %f\n", maxlen, msg.str, expr->ans);
        free(msg.str);
}
#undef cur_op
#undef prev_op
