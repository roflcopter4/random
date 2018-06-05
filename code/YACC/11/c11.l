%e  1019
%p  2807
%n  371
%k  284
%a  1213
%o  1117

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]

%{
#include <stdio.h>
#include "y.tab.h"

extern void yyerror(const char *);  /* prints grammar violation message */

extern int sym_type(const char *);  /* returns type from symbol table */

#define sym_type(identifier) IDENTIFIER /* with no symbol table, fake it */

static void comment(void);
static int check_type(void);
%}

%%
"/*"                                    { ECHO; putchar('\n'); comment(); }
"//".*                                    { /* consume //-comment */ }

"auto"					{ ECHO; putchar('\n'); return(AUTO); }
"break"					{ ECHO; putchar('\n'); return(BREAK); }
"case"					{ ECHO; putchar('\n'); return(CASE); }
"char"					{ ECHO; putchar('\n'); return(CHAR); }
"const"					{ ECHO; putchar('\n'); return(CONST); }
"continue"				{ ECHO; putchar('\n'); return(CONTINUE); }
"default"				{ ECHO; putchar('\n'); return(DEFAULT); }
"do"					{ ECHO; putchar('\n'); return(DO); }
"double"				{ ECHO; putchar('\n'); return(DOUBLE); }
"else"					{ ECHO; putchar('\n'); return(ELSE); }
"enum"					{ ECHO; putchar('\n'); return(ENUM); }
"extern"				{ ECHO; putchar('\n'); return(EXTERN); }
"float"					{ ECHO; putchar('\n'); return(FLOAT); }
"for"					{ ECHO; putchar('\n'); return(FOR); }
"goto"					{ ECHO; putchar('\n'); return(GOTO); }
"if"					{ ECHO; putchar('\n'); return(IF); }
"inline"				{ ECHO; putchar('\n'); return(INLINE); }
"int"					{ ECHO; putchar('\n'); return(INT); }
"long"					{ ECHO; putchar('\n'); return(LONG); }
"register"				{ ECHO; putchar('\n'); return(REGISTER); }
"restrict"				{ ECHO; putchar('\n'); return(RESTRICT); }
"return"				{ ECHO; putchar('\n'); return(RETURN); }
"short"					{ ECHO; putchar('\n'); return(SHORT); }
"signed"				{ ECHO; putchar('\n'); return(SIGNED); }
"sizeof"				{ ECHO; putchar('\n'); return(SIZEOF); }
"static"				{ ECHO; putchar('\n'); return(STATIC); }
"struct"				{ ECHO; putchar('\n'); return(STRUCT); }
"switch"				{ ECHO; putchar('\n'); return(SWITCH); }
"typedef"				{ ECHO; putchar('\n'); return(TYPEDEF); }
"union"					{ ECHO; putchar('\n'); return(UNION); }
"unsigned"				{ ECHO; putchar('\n'); return(UNSIGNED); }
"void"					{ ECHO; putchar('\n'); return(VOID); }
"volatile"				{ ECHO; putchar('\n'); return(VOLATILE); }
"while"					{ ECHO; putchar('\n'); return(WHILE); }
"_Alignas"                              { ECHO; putchar('\n'); return ALIGNAS; }
"_Alignof"                              { ECHO; putchar('\n'); return ALIGNOF; }
"_Atomic"                               { ECHO; putchar('\n'); return ATOMIC; }
"_Bool"                                 { ECHO; putchar('\n'); return BOOL; }
"_Complex"                              { ECHO; putchar('\n'); return COMPLEX; }
"_Generic"                              { ECHO; putchar('\n'); return GENERIC; }
"_Imaginary"                            { ECHO; putchar('\n'); return IMAGINARY; }
"_Noreturn"                             { ECHO; putchar('\n'); return NORETURN; }
"_Static_assert"                        { ECHO; putchar('\n'); return STATIC_ASSERT; }
"_Thread_local"                         { ECHO; putchar('\n'); return THREAD_LOCAL; }
"__func__"                              { ECHO; putchar('\n'); return FUNC_NAME; }

{L}{A}*					{ ECHO; putchar('\n'); return check_type(); }

{HP}{H}+{IS}?				{ ECHO; putchar('\n'); return I_CONSTANT; }
{NZ}{D}*{IS}?				{ ECHO; putchar('\n'); return I_CONSTANT; }
"0"{O}*{IS}?				{ ECHO; putchar('\n'); return I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ ECHO; putchar('\n'); return I_CONSTANT; }

{D}+{E}{FS}?				{ ECHO; putchar('\n'); return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			{ ECHO; putchar('\n'); return F_CONSTANT; }
{D}+"."{E}?{FS}?			{ ECHO; putchar('\n'); return F_CONSTANT; }
{HP}{H}+{P}{FS}?			{ ECHO; putchar('\n'); return F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			{ ECHO; putchar('\n'); return F_CONSTANT; }
{HP}{H}+"."{P}{FS}?			{ ECHO; putchar('\n'); return F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ ECHO; putchar('\n'); return STRING_LITERAL; }

"..."					{ ECHO; putchar('\n'); return ELLIPSIS; }
">>="					{ ECHO; putchar('\n'); return RIGHT_ASSIGN; }
"<<="					{ ECHO; putchar('\n'); return LEFT_ASSIGN; }
"+="					{ ECHO; putchar('\n'); return ADD_ASSIGN; }
"-="					{ ECHO; putchar('\n'); return SUB_ASSIGN; }
"*="					{ ECHO; putchar('\n'); return MUL_ASSIGN; }
"/="					{ ECHO; putchar('\n'); return DIV_ASSIGN; }
"%="					{ ECHO; putchar('\n'); return MOD_ASSIGN; }
"&="					{ ECHO; putchar('\n'); return AND_ASSIGN; }
"^="					{ ECHO; putchar('\n'); return XOR_ASSIGN; }
"|="					{ ECHO; putchar('\n'); return OR_ASSIGN; }
">>"					{ ECHO; putchar('\n'); return RIGHT_OP; }
"<<"					{ ECHO; putchar('\n'); return LEFT_OP; }
"++"					{ ECHO; putchar('\n'); return INC_OP; }
"--"					{ ECHO; putchar('\n'); return DEC_OP; }
"->"					{ ECHO; putchar('\n'); return PTR_OP; }
"&&"					{ ECHO; putchar('\n'); return AND_OP; }
"||"					{ ECHO; putchar('\n'); return OR_OP; }
"<="					{ ECHO; putchar('\n'); return LE_OP; }
">="					{ ECHO; putchar('\n'); return GE_OP; }
"=="					{ ECHO; putchar('\n'); return EQ_OP; }
"!="					{ ECHO; putchar('\n'); return NE_OP; }
";"					{ ECHO; putchar('\n'); return ';'; }
("{"|"<%")				{ ECHO; putchar('\n'); return '{'; }
("}"|"%>")				{ ECHO; putchar('\n'); return '}'; }
","					{ ECHO; putchar('\n'); return ','; }
":"					{ ECHO; putchar('\n'); return ':'; }
"="					{ ECHO; putchar('\n'); return '='; }
"("					{ ECHO; putchar('\n'); return '('; }
")"					{ ECHO; putchar('\n'); return ')'; }
("["|"<:")				{ ECHO; putchar('\n'); return '['; }
("]"|":>")				{ ECHO; putchar('\n'); return ']'; }
"."					{ ECHO; putchar('\n'); return '.'; }
"&"					{ ECHO; putchar('\n'); return '&'; }
"!"					{ ECHO; putchar('\n'); return '!'; }
"~"					{ ECHO; putchar('\n'); return '~'; }
"-"					{ ECHO; putchar('\n'); return '-'; }
"+"					{ ECHO; putchar('\n'); return '+'; }
"*"					{ ECHO; putchar('\n'); return '*'; }
"/"					{ ECHO; putchar('\n'); return '/'; }
"%"					{ ECHO; putchar('\n'); return '%'; }
"<"					{ ECHO; putchar('\n'); return '<'; }
">"					{ ECHO; putchar('\n'); return '>'; }
"^"					{ ECHO; putchar('\n'); return '^'; }
"|"					{ ECHO; putchar('\n'); return '|'; }
"?"					{ ECHO; putchar('\n'); return '?'; }

{WS}+					{ /* whitespace separates tokens */ }
.					{ /* discard bad characters */ }

%%

int yywrap(void)        /* called at end of input */
{
        return 1;           /* terminate now */
}

static void comment(void)
{
        int c;

        while ((c = input()) != 0) {
                if (c == '*') {
                        while ((c = input()) == '*')
                                ;

                        if (c == '/')
                                return;

                        if (c == 0)
                                break;
                }
        }
        yyerror("unterminated comment");
}

static int check_type(void)
{
        switch (sym_type(yytext)) {
        case TYPEDEF_NAME:                /* previously defined */
                return TYPEDEF_NAME;
        case ENUMERATION_CONSTANT:        /* previously defined */
                return ENUMERATION_CONSTANT;
        default:                          /* includes undefined */
                return IDENTIFIER;
        }
}
