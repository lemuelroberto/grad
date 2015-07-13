/* Command-line processing */

/* Ref. http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt */

#ifndef __OPT_H__
#define __OPT_H__

typedef struct {
  int help_opt;
  int input_opt;
  int output_opt;

  int laplacian_opt;
  int gaussian_opt;

  char *input_optarg;
  char *output_optarg;
} Argument;

void createArgument(Argument *args);
int commandProcessing(int argc, char *argv[], const char *opt_string, Argument *args);
void print_help(char *program);

#endif // __OPT_H_INCLUDED__
