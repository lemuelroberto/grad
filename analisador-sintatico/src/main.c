#include <stdio.h>
#include <stdarg.h>

#include "../inc/opt.h"
#include "../inc/yacc.yy.h"

extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s, ...) {
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d:%d: error: ", yylloc.first_line, yylloc.first_column);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int main(int argc, char *argv[]) {
  Argument args;

  FILE *in_file = NULL;

  /* command-line processing */
  if(commandProcessing(argc, argv, "hi:", &args) != 0) {
    return 1;
  }

  if(args.input_optarg != NULL) { /* check for input file */
    in_file = fopen(args.input_optarg, "rb");
    if(in_file == NULL) {
      fprintf(stderr, "%s: fatal error: couldn't open %s\n", argv[0],
        args.input_optarg);
      return 1;
    }
  } else {
    in_file = stdin;
  }

  yyin = in_file;

  /* parser */
  yyparse();

  if(in_file != NULL) fclose(in_file);

  return 0;
}
