#include "fool.h"
#include <bsd/bsd.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define add_operand(NODE, BUF, IND)                      \
        do {                                             \
                if ((IND) > 0) {                         \
                        (BUF)[IND] = '\0';               \
                        _add_operand((NODE), atof(BUF)); \
                }                                        \
        } while (0)

#define modulo(A, B) (((A) % (B) + (B)) % (B))
#define fmodulo(A, B) fmod((fmod((A), (B)) + (B)), (B))

static void
parse_line(struct expression *expr, const char *line, uint len, bool in_paren);

static struct Child *solve(struct expression *expr);
static double do_operation(enum Operators operator, double a, double b);
static inline int get_precidence(enum Operators ch);
static inline int find_max_precidence(const enum Operators *ops, uint numops);

static void format_line(char *buf, const struct expression *expr, uint16_t prec);
static inline void format_operand(char *buf, double ans, uint16_t prec);
static inline void format_operator(char *buf, int op);


int
main(int argc, char **argv)
{
        program_name = argv[0];
        handle_options(argc, argv);
        struct str_list *lines = getlines(argv[1]);

        for (uint i = 0; i < lines->num; ++i) {
                struct expression *root = init_tree();
                parse_line(root, lines->data[i], lines->sizes[i], false);

                char normalized[BUFSIZ] = "";
                format_line(normalized, root, 3);

                struct Child *result = solve(root);
                printf("%9.3f =  %s\n", result->u.ans, normalized);

                destroy_child(result);
        }

        destroy_str_list(lines);
        return 0;
}


static void
parse_line(struct expression *expr,
           const char *const line,
           const uint len,
           const bool in_paren)
{
        bool dash = false, token = false;
        int paren = 0, bufind = 0;
        char buf[len];
        buf[0] = '\0';

        uint i = 0;
        for (i = 0; i < len; ++i) {
                int ch = line[i];
                if (!token)
                        buf[bufind = 0] = '\0';
                if (paren == 0)
                        if (isdigit(ch) || ch == '.') {
                                if (dash) {
                                        buf[bufind++] = '-';
                                        dash = false;
                                }
                                buf[bufind++] = (char)ch;
                                token = true;
                        } else {
                                switch (ch) {
                                case '(':
                                        ++paren;
                                        if (dash) {
                                                add_operator(expr, '-');
                                                dash = false;
                                        }
                                        token = true;
                                        parse_line(add_expr(expr, in_paren),
                                                   line+i+1, len-1, true);
                                        break;

                                case ')':
                                        goto done;

                                case '-':
                                        if (dash) {
                                                add_operator(expr, '-');
                                                token = false;
                                        } else if (token) {
                                                add_operand(expr, buf, bufind);
                                                add_operator(expr, '-');
                                                dash = token = false;
                                        } else {
                                                dash = true;
                                        }
                                        break;

                                case '+':
                                case '*':
                                case '/':
                                case '%':
                                case '^':
                                        if (token) {
                                                add_operand(expr, buf, bufind);
                                                token = false;
                                        }
                                        add_operator(expr, ch);
                                        break;
                                }
                        }
                else if (ch == ')')
                        --paren;
                else if (ch == '(')
                        ++paren;
        }

done:
        if (token)
                add_operand(expr, buf, bufind);
}


static struct Child *
solve(struct expression *expr)
{
#define CHILD        expr->child
#define operands(IT) expr->child[IT]->u.ans
#define operators    expr->ops

        /* for (uint i = 0; i < expr->nchild; ++i)
                if (expr->child[i]->tag == EXPR) {
                        struct Child *tmp = solve(expr->child[i]->u.expr);
                        free(expr->child[i]);
                        expr->child[i] = tmp;
                }  */
        for (uint i = 0; i < expr->nchild; ++i)
                if (CHILD[i]->tag == EXPR) {
                        struct Child *tmp = solve(CHILD[i]->u.expr);
                        free(CHILD[i]);
                        CHILD[i] = tmp;
                }

        (*expr).nchild++;
        double ans;
        uint num_operands = expr->numops;

        for (uint num = expr->numops; num > 0; --num) {
                int max = find_max_precidence(operators, num);
                for (uint x = 0; x < num; ++x) {
                        enum Operators op = operators[x];
                        int p;
                        if ((p = get_precidence(op)) == max && p > 0) {
                                double a, b;
                                a = operands(x);
                                b = operands(x + 1);
                                ans = do_operation(op, a, b);

                                operands(x) = ans;
                                for (uint n = x + 1; n < num_operands; ++n)
                                        operands(n) = operands(n + 1);
                                --num_operands;

                                for (uint n = x; n < num - 1; ++n)
                                        operators[n] = operators[n + 1];

                                break;
                        }
                }
        }

#undef CHILD
#undef operands
#undef operators
        return replace_node(expr, ans);
}


static double
do_operation(const enum Operators operator, const double a, const double b)
{
        switch (operator) {
        case Add:
                return a + b;
        case Subtract:
                return a - b;
        case Multiply:
                return a * b;
        case Divide:
                return a / b;
        case Modulo:
                return fmodulo(a, b);
        case Power:
                return pow(a, b);
        case None:
                xeprintf(1, "wtf how is this even possible.\n");
        }
        return 0; /* This shouldn't be reachable but it shuts up gcc. */
}


static inline int
get_precidence(const enum Operators ch)
{
        switch (ch) {
        case Add:
        case Subtract:
                return 1;
        case Multiply:
        case Divide:
        case Modulo:
                return 2;
        case Power:
                return 3;
        case None:
                return 0;
        default:
                xeprintf(40, "WTF\n");
        }
}


static inline int
find_max_precidence(const enum Operators *const ops, const uint numops)
{
        int max = 0;
        for (uint i = 0; i < numops; ++i) {
                int tmp = get_precidence(ops[i]);
                max = (tmp > max) ? tmp : max;
        }

        return max;
}


static void
format_line(char *buf,
            const struct expression *const expr,
            const uint16_t prec)
{
        uint optype_i = 0;
        /* if (expr->in_paren && expr->nchild > 1) */
                strlcat(buf, "(", BUFSIZ);

#define CHILD expr->child[n]

        for (uint n = 0; n < expr->nchild; ++n) {
                if (CHILD->tag == EXPR)
                        format_line(buf, CHILD->u.expr, prec);
                else
                        format_operand(buf, CHILD->u.ans, prec);

                if (optype_i < expr->numops)
                        format_operator(buf, expr->ops[optype_i++]);
        }

        /* if (expr->in_paren && expr->nchild > 1) */
                strlcat(buf, ")", BUFSIZ);

#undef CHILD
}


static inline void
format_operand(char *buf, const double ans, const uint16_t prec)
{
        char tmp[BUFSIZ];

        if (floor(ans) == ans)
                snprintf(tmp, BUFSIZ, "%d", (int)ans);
        else
                snprintf(tmp, BUFSIZ, "%.*f", prec, ans);

        strlcat(buf, tmp, BUFSIZ);
}


static inline void
format_operator(char *buf, const int op)
{
        char tmp[1024];
        snprintf(tmp, 1024, " %c ", (char)op);
        strlcat(buf, tmp, BUFSIZ);
}
