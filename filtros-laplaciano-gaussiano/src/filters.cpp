#include "../inc/filters.hpp"

void convoluteImage(Mat& srcImage, Mat& ker, unsigned int ksize) {

  Mat filteringImage = srcImage.clone();
  unsigned int kradius = (unsigned int)(ksize / 2);

  copyMakeBorder( srcImage, filteringImage, kradius, kradius, kradius, kradius,
    BORDER_CONSTANT, Scalar(255) );

  for(unsigned int i = kradius; i < srcImage.rows + kradius; ++i) {
    for(unsigned int j = kradius; j < srcImage.cols + kradius; ++j) {
      double sum = 0.0;

      for(unsigned int k = 0; k < ksize; ++k) {
        for(unsigned int l = 0; l < ksize; ++l) {
          sum += (double)filteringImage.at<uchar>(i-kradius+k, j-kradius+l)
                          * (double)(ker).at<double>(k, l);
        }
      }

      sum = (sum < 0) ? 0 : sum;

      srcImage.at<uchar>(i-kradius, j-kradius) = (uchar)sum;
    }
  }
}

void Laplacian(const Mat& src, Mat& dst, unsigned int ksize) {

  /* variable declaration */
  Mat ker;

  CV_Assert(src.depth() == CV_8U); // accept only uchar images

  src.copyTo(dst); // copy src into dst

  createLaplacianKernel(ker); // create kernel

  convoluteImage(dst, ker, ksize); // convolute image
}

void Gaussian(const Mat& src, Mat& dst, unsigned int ksize, double sigma) {

  /* variable declaration */
  Mat ker;

  CV_Assert(src.depth() == CV_8U); // accept only uchar images

  src.copyTo(dst); // copy src into dst

  createGaussianKernel(ker, ksize, sigma); // create kernel

  convoluteImage(dst, ker, ksize); // convolute image
}

void LoG(const Mat& src, Mat& dst, unsigned int gksize, double sigma,
  unsigned int lksize) {

  Mat tmp;

  Gaussian(src, dst, gksize, sigma);

  Laplacian(dst, tmp, lksize);

  tmp.copyTo(dst);
}

void createLaplacianKernel(Mat& ker) {

  ker = (Mat_<double>(3,3) <<   0,  -1,  0,
                              -1,  4,  -1,
                              0,  -1,  0);
}

void createGaussianKernel(Mat& ker, unsigned int ksize, double sigma) {

  double gaussianValue = 0.0;
  double sum = 0.0;
  double auxSigma = 2.0 * pow(sigma, 2.0);


  ker.create(ksize, ksize, CV_64F);

  for(unsigned int i = 0; i < ksize; ++i) {
    for(unsigned int j = 0; j < ksize; ++j) {
      gaussianValue = ( exp( ( pow(i, 2.0) + pow(j, 2.0) ) / (-auxSigma) )
                        / ( M_PI * auxSigma ) );

      ker.at<double>(i,j) = gaussianValue;

      sum += gaussianValue;
    }
  }

  for(unsigned int i = 0; i < ksize; ++i) { // normalize the kernel
    for(unsigned int j = 0; j < ksize; ++j) {
      ker.at<double>(i,j) /= sum;
    }
  }
}
