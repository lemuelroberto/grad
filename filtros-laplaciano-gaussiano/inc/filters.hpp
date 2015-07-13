#ifndef __FILTERS_HPP__
#define __FILTERS_HPP__

#include <cv.h>

using namespace cv;

void convoluteImage(Mat& orginalImage, Mat& ker, unsigned int ksize);

void Laplacian(const Mat& src, Mat& dst, unsigned int ksize);
void createLaplacianKernel(Mat& ker);


void Gaussian(const Mat& src, Mat& dst, unsigned int ksize, double sigma);
void createGaussianKernel(Mat& ker, unsigned int ksize, double sigma);

void LoG(const Mat& src, Mat& dst, unsigned int gksize, double sigma, unsigned int lksize);

#endif // __FILTERS_HPP_INCLUDED__
