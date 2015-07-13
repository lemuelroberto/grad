#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

#include "../inc/opt.h"

void createArgument(Argument *args) {

  args->help_opt = 0;
  args->input_opt = 0;
  args->output_opt = 0;

  args->laplacian_opt = 0;
  args->gaussian_opt = 0;

  args->input_optarg = NULL;
  args->output_optarg = NULL;
}

int commandProcessing(int argc, char *argv[], const char *opt_string,
  Argument *args) {

  int option = -1;

  createArgument(args);

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
      case 'o': /* out file */
        args->output_opt = 1;
        args->output_optarg = optarg;
        break;
      case 'l': /* out file */
        args->laplacian_opt = 1;
        break;
      case 'g': /* out file */
        args->gaussian_opt = 1;
        break;
      case '?': /* error */
        if (optopt == 'i' || optopt == 'o') {
          fprintf (stderr, "Option -%c requires an argument\n", optopt);
        }
        else if (isprint (optopt)) {
          fprintf (stderr, "Unknown option '-%c'\n", optopt);
        }
        else {
          fprintf (stderr, "Unknown option character '\\x%x'\n", optopt);
        }
        fprintf(stderr, "Try \'%s -h\' for help.\n", argv[0]);
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

  printf("Usage: %s [FILTER]... FILE...\n", program);
  printf("A didactic implementation of Laplacian, Gaussian and of LoG filters.\n\n");

  printf("Filters:\n");
  printf("\t-g\t\tapply Gaussian filter to the image\n");
  printf("\t-l\t\tapply Laplacian filter to the image\n");
  printf("\t-lg\t\tapply Laplacian of Gaussian filter to the image\n");

  printf("Files:\n");
  printf("\t-i FILE\t\tspecify input image file name\n");
  printf("\t-o FILE\t\tspecify output image file name\n");

  printf("Miscellaneous:\n");
  printf("\t-h\t\tproduce this help message\n");
}
