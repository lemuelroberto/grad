%{

#define YYERROR_VERBOSE 1

extern int yylex();

extern char *yytext;

void yyerror(const char *s, ...);

%}

%locations

%token PAR_OPEN
%token PAR_CLOSE
%token COMMA
%token SEMICOLON
%token PERIOD
%token COLON
%token EQUAL
%token NOT_EQUAL
%token GREATER_EQUAL
%token LESS_EQUAL
%token GREATER
%token LESS
%token PLUS
%token MINUS
%token ASTERISK
%token SLASH

%token PROGRAM
%token BEGIN_TOKEN
%token END
%token CONST
%token VAR
%token INTEGER
%token REAL
%token PROCEDURE
%token READ
%token WRITE
%token WHILE
%token IF
%token THEN
%token ELSE
%token FOR
%token DO

%token IDENT

%token REAL_NUMBER
%token INTEGER_NUMBER

%token error

%start input_file

%%

input_file
  :
  | programa
;

programa
  : PROGRAM IDENT SEMICOLON corpo PERIOD
  | error SEMICOLON {yyerrok;} corpo PERIOD
;

corpo
  : dc BEGIN_TOKEN comandos END
  | dc error {yyerrok;} comandos END
;

dc
  : dc_c dc_v dc_p
;

dc_c
  :
  | CONST IDENT EQUAL numero SEMICOLON dc_c
  | CONST error SEMICOLON {yyerrok;} dc_c

;

dc_v
  :
  | VAR variaveis COLON tipo_var SEMICOLON dc_v
  | VAR error SEMICOLON {yyerrok;} dc_v
;

tipo_var
  : REAL
  | INTEGER
;

variaveis
  : IDENT mais_var
;

mais_var
  :
  | COMMA variaveis
;

dc_p
  :
  | PROCEDURE IDENT parametros SEMICOLON corpo_p dc_p
  | PROCEDURE error SEMICOLON {yyerrok;} corpo_p dc_p
;

parametros
  :
  | PAR_OPEN lista_par PAR_CLOSE
;

lista_par
  : variaveis COLON tipo_var mais_par
;

mais_par
  :
  | SEMICOLON lista_par
;

corpo_p
  : dc_loc BEGIN_TOKEN comandos END SEMICOLON
  | error SEMICOLON {yyerrok;}
;

dc_loc
  : dc_v
;

lista_arg
  :
  | PAR_OPEN argumentos PAR_CLOSE
;

argumentos
  : IDENT mais_ident
;

mais_ident
  :
  | SEMICOLON argumentos
;

pfalsa
  :
  | ELSE cmd
;

comandos
  :
  | cmd SEMICOLON comandos
  | error SEMICOLON {yyerrok;} comandos
;

cmd
  : READ PAR_OPEN variaveis PAR_CLOSE
  | WRITE PAR_OPEN variaveis PAR_CLOSE
  | WHILE PAR_OPEN condicao PAR_CLOSE DO cmd
  | IF condicao THEN cmd pfalsa
  | IDENT COLON EQUAL expressao
  | IDENT lista_arg
  | BEGIN_TOKEN comandos END
;

condicao
  : expressao relacao expressao
;

relacao
  : EQUAL
  | NOT_EQUAL
  | GREATER_EQUAL
  | LESS_EQUAL
  | GREATER
  | LESS
;

expressao
  : termo outros_termos
;

op_un
  :
  | PLUS
  | MINUS
;

outros_termos
  :
  | op_ad termo outros_termos
;

op_ad
  : PLUS
  | MINUS
;

termo
  : op_un fator mais_fatores
;

mais_fatores
  :
  | op_mul fator mais_fatores
;

op_mul
  : ASTERISK
  | SLASH
;

fator
  : IDENT
  | numero
  | PAR_OPEN expressao PAR_CLOSE
;

numero
  : INTEGER_NUMBER
  | REAL_NUMBER
;

%%
