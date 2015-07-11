#include "../inc/opt.h"

#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

int commandProcessing(int argc, char *argv[], const char *opt_string, Argument *args) {
  int option = -1;

  while((option = getopt(argc, argv, opt_string)) != -1) {
    switch(option) {
      case 'h': /* help */
        args->help_opt = 1;
        print_help(argv[0]);
        return 1; /* ends the program */
      case 'i': /* input file */
        args->input_opt = 1;
        args->input_optarg = optarg;
        break;
      case '?': /* error */
        if (optopt == 'i') {
          fprintf (stderr, "Option -%c requires an argument\n", optopt);
        }
        else if (isprint (optopt)) {
          fprintf (stderr, "Unknown option '-%c'\n", optopt);
        }
        else {
          fprintf (stderr, "Unknown option character '\\x%x'\n", optopt);
        }
        fprintf(stderr, "\nRun %s -h for help.\n", argv[0]);
        return -1;
    }
  }

  int i;
  for (i = optind; i < argc; i++) { /* error */
    fprintf (stderr, "Non-option argument %s\n", argv[i]);
    fprintf(stderr, "\nRun %s -h for help.\n", argv[0]);
    return -1;
  }

  return 0;
}

void print_help(char *program) {
  printf("Usage: %s [OPTIONS] [FILE]...\n", program);
  printf("A didactic compiler for LALG. For now, just the scanner and parser.\n\n");
  printf("Files:\n");
  printf("\t-i FILE\t\tspecify input filename (default stdin)\n");

  printf("Miscellaneous:\n");
  printf("\t-h\t\tproduce this help message\n");
}
