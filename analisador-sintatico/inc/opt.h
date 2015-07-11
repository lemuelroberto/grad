/* Command-line processing */

/* Ref. http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt */

#ifndef __OPT_H__
#define __OPT_H__

#include <stdlib.h>

typedef struct {
  int help_opt = 0;
  int input_opt = 0;

  char *input_optarg = NULL;
} Argument;

int commandProcessing(int argc, char *argv[], const char *opt_string, Argument *args);
void print_help(char *program);

#endif // __OPT_H_INCLUDED__
