/* test.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c__3 = 3;
static integer c__2 = 2;

/* Main program */ int MAIN__(void)
{
        /* Format strings */
        static char fmt_1000[] = "(i1,a,i1)";

        /* Builtin functions */
        integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen),
                e_wsfe(void), s_wsle(cilist *), do_lio(integer *, integer *,
                                char *, ftnlen), e_wsle(void);

        /* Local variables */
        static integer ia, ib, ic[2];

        /* Fortran I/O blocks */
        static cilist io___4 = {0, 6, 0, fmt_1000, 0};
        static cilist io___5 = {0, 6, 0, fmt_1000, 0};
        static cilist io___6 = {0, 6, 0, fmt_1000, 0};
        static cilist io___7 = {0, 6, 0, 0, 0};


        ia = 2;
        ib = 1;
        ic[0] = ia;
        ic[1] = ib;
        /* L1000: */
        if (ia < ib) {
                s_wsfe(&io___4);
                do_fio(&c__1, (char *)&ia, (ftnlen)sizeof(integer));
                do_fio(&c__1, " is less than ", (ftnlen)14);
                do_fio(&c__1, (char *)&ib, (ftnlen)sizeof(integer));
                e_wsfe();
        } else if (ia == ib) {
                s_wsfe(&io___5);
                do_fio(&c__1, (char *)&ia, (ftnlen)sizeof(integer));
                do_fio(&c__1, " is equal to ", (ftnlen)13);
                do_fio(&c__1, (char *)&ib, (ftnlen)sizeof(integer));
                e_wsfe();
        } else {
                s_wsfe(&io___6);
                do_fio(&c__1, (char *)&ia, (ftnlen)sizeof(integer));
                do_fio(&c__1, " is greater than ", (ftnlen)17);
                do_fio(&c__1, (char *)&ib, (ftnlen)sizeof(integer));
                e_wsfe();
        }
        s_wsle(&io___7);
        do_lio(&c__3, &c__2, (char *)&ic[0], (ftnlen)sizeof(integer));
        e_wsle();
        return 0;
} /* MAIN__ */

/* Main program alias */ int test_() { MAIN__(); return 0; }
