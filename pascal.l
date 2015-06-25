%{
  /*
   * pascal.l
   */	

#include <stdio.h>
#include "y.tab.h"
int line_number = 1;

extern int error_flag;

void commenteof();

%}

A [aA]
B [bB]
C [cC]
D [dD]
E [eE]
F [fF]
G [gG]
H [hH]
I [iI]
J [jJ]
K [kK]
L [lL]
M [mM]
N [nN]
O [oO]
P [pP]
Q [qQ]
R [rR]
S [sS]
T [tT]
U [uU]
V [vV]
W [wW]
X [xX]
Y [yY]
Z [zZ]
NQUOTE [^']

%%

{A}{N}{D}   return(AND);
{A}{R}{R}{A}{Y}   return(ARRAY);
{C}{L}{A}{S}{S}   return(CLASS);
{D}{O}    return(DO);
{E}{L}{S}{E}   return(ELSE);
{E}{N}{D}   return(END);
{E}{X}{T}{E}{N}{D}{S}   return(EXTENDS);
{F}{U}{N}{C}{T}{I}{O}{N} return(FUNCTION);
{I}{F}    return(IF);
{M}{O}{D}   return(MOD);
{N}{E}{W}    return(NEW);
{N}{O}{T}   return(NOT);
{O}{F}    return(OF);
{O}{R}    return(OR);
{P}{R}{I}{N}{T}  return(PRINT);
{B}{E}{G}{I}{N}   return(PBEGIN);
{P}{R}{O}{G}{R}{A}{M}  return(PROGRAM);
{T}{H}{E}{N}   return(THEN);
{V}{A}{R}   return(VAR);
{W}{H}{I}{L}{E}   return(WHILE);
[a-zA-Z]([a-zA-Z0-9])+  return(IDENTIFIER);

":="    return(ASSIGNMENT);
":"    return(COLON);
","    return(COMMA);
[0-9]+    return(DIGSEQ);
"."    return(DOT);
".."    return(DOTDOT);
"="    return(EQUAL);
">="    return(GE);
">"    return(GT);
"["    return(LBRAC);
"<="    return(LE);
"("    return(LPAREN);
"<"    return(LT);
"-"    return(MINUS);
"<>"    return(NOTEQUAL);
"+"    return(PLUS);
"]"    return(RBRAC);
")"    return(RPAREN);
";"    return(SEMICOLON);
"/"    return(SLASH);
"*"    return(STAR);
"(*"   |
"{"    { register int c;
     while ((c = input()))
     {
      if (c == '}')
       break;
      else if (c == '*')
      {
       if ((c = input()) == ')')
        break;
       else
        unput (c);
      }
      else if (c == '\n')
       line_number++;
      else if (c == 0)
       commenteof();
     }
    }

[ \t\f]    ;

\n    line_number++;

.    { fprintf (stderr,
    "'%c' (0%o): illegal character at line %d\n",
     yytext[0], yytext[0], line_number);
    }

%%

void commenteof()
{
 fprintf (stderr, "unexpected EOF inside comment at line %d\n", line_number);
 exit (1);
}

int yywrap ()
{
 return 1;
}

void yyerror(const char *error)
{
  printf("%d: %s at '%s'\n", line_number, error, yytext);
  error_flag = 1;
}
