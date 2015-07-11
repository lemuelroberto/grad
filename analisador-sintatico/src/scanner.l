%{

#include "../inc/yacc.yy.h"

/* handle locations

  Ref. John, Levine - flex & bison, Text Processing Tools

*/
extern void yyerror(const char *s, ...);
extern char *yytext;

int yycolumn = 1;

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;

%}

%option yylineno
%option nounput

%%

"("                     return PAR_OPEN;
")"                     return PAR_CLOSE;
","                     return COMMA;
";"                     return SEMICOLON;
"."                     return PERIOD;
":"                     return COLON;
"="                     return EQUAL;
"<>"                    return NOT_EQUAL;
">="                    return GREATER_EQUAL;
"<="                    return LESS_EQUAL;
">"                     return GREATER;
"<"                     return LESS;
"+"                     return PLUS;
"-"                     return MINUS;
"*"                     return ASTERISK;
"/"                     return SLASH;

"program"   						return PROGRAM;
"begin"     						return BEGIN_TOKEN;
"end"       						return END;
"const"     						return CONST;
"var"					        	return VAR;
"integer"				     		return INTEGER;
"real"						      return REAL;
"procedure"             return PROCEDURE;
"read"						      return READ;
"write"                 return WRITE;
"while"                 return WHILE;
"if"                    return IF;
"then"                  return THEN;
"else"                  return ELSE;
"for"                   return FOR;
"do"                    return DO;

[a-zA-Z][_a-zA-Z0-9]{0,126}   return IDENT;
[a-zA-Z][_a-zA-Z0-9]*         yyerror("identifier %s is too long", yytext);

[0-9]+"."[0-9]+         return REAL_NUMBER;
[0-9]+                  return INTEGER_NUMBER;

[\ \t]                  ;
[\n\r]                  yycolumn = 1;
"{".*"}"                ;

.                       yyerror("stray %s in program", yytext);

%%

extern int yywrap(void) {
  return 1;
}
