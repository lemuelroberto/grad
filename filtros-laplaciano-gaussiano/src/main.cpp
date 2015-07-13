/*
  Trabalho 2 - Filtros Laplaciano, Gaussiano e Laplaciano do Gaussiano

  Aluno: Lemuel Roberto Bonifácio
  NUSP: 7277728
*/

/* OpenCV headers */
#include <highgui.h>

/* i/o and standard headers */
#include <stdlib.h>
#include <stdio.h>

/* debug and command line headers */
#include "../inc/opt.h"
#include "../inc/dbg.h"
#include "../inc/filters.hpp"

int main( int argc, char** argv ) {
  /* opencv variables */
  Mat originalImage, filteredImage;

  /* command line variables */
  Argument args;

  char *inFile = NULL;
  char *outFile = NULL;

  /* command-line processing */
  if(commandProcessing(argc, argv, "hlgi:o:", &args) != 0) {
    return -1;
  }

  if(args.input_optarg != NULL) { /* check for input file */
    inFile = args.input_optarg;

    // TODO: open input file
    debug("input file: %s", inFile);
  } else {
    fprintf(stderr, "%s: missing input file operand\n", argv[0]);
    fprintf(stderr, "Try \'%s -h\' for help.\n", argv[0]);

    return -1;
  }

  if(args.output_optarg != NULL) { /* check for output file */
    outFile = args.output_optarg;

    // TODO: open output file
    debug("output file: %s", outFile);
  } else {
    fprintf(stderr, "%s: missing output file operand\n", argv[0]);
    fprintf(stderr, "Try \'%s -h\' for help.\n", argv[0]);

    return -1;
  }

  originalImage = imread(inFile, IMREAD_GRAYSCALE);

  if(originalImage.data == NULL) {
    fprintf(stderr, "%s: fatal error: couldn't open %s\n", argv[0],
              args.input_optarg);

    return -1;
  }

  /* check for filter option */
  if(args.laplacian_opt == 1
      && args.gaussian_opt == 1) { /* check for Laplacian of Gaussian filter */

    debug("LoG filter");

    // TODO: apply laplacian filter
    LoG(originalImage, filteredImage, (unsigned int) 3, 1.0, (unsigned int) 3);
  } else if(args.laplacian_opt == 1) { /* check for Laplacian filter */
    debug("Laplacian filter");

    // TODO: apply Laplacian filter
    Laplacian(originalImage, filteredImage, (unsigned int) 3);

  } else if(args.gaussian_opt == 1) { /* check for Gaussian filter */
    debug("Gaussian filter");

    // TODO: apply Gaussian filter
    Gaussian(originalImage, filteredImage, (unsigned int) 3, 1.0);
  } else {
    debug("none filter apllyed");
  }

  if(imwrite(outFile, filteredImage) == false) {
    fprintf(stderr, "%s: fatal error: couldn't save %s\n", argv[0],
            args.output_optarg);

    return -1;
  }

  #ifndef NDEBUG
    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", originalImage);

    namedWindow("Filtered", WINDOW_AUTOSIZE);
    imshow("Filtered", filteredImage);

    waitKey(0);
  #endif

  return 0;
}
