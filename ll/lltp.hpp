#pragma once

// define below macro to use them before #include
// #define LL_USE_OPENCV
// #define LL_USE_EIGEN

// 
#ifdef LL_USE_OPENCV
#include <opencv2/opencv.hpp>

#define LL_IMSHOW(img) cv::imshow(#img, img)

namespace ll {
namespace tp {

void plot_line(
    cv::Mat& img, cv::Point2f p0, cv::Point2f p1, const cv::Scalar& color) {
  // simple y=kx+b
  // todo: handle vertical line
  float k = (p1.y - p0.y) / (p1.x - p0.x);
  float b = -k * p0.x + p0.y;

  cv::Point lft(0, 0), rt(img.cols, 0);
  lft.y = b;
  rt.y = k * rt.x + b;
  cv::line(img, lft, rt, color);
};
}
}
#endif

#ifdef LL_USE_EIGEN
namespace ll {
namespace tp {

#include <Eigen/Dense>

template<int AR, int AC, int BR, int BC, typename Scalar=double>
Eigen::Matrix<Scalar, AR*BR, AC*BC> kronecker(const Eigen::Matrix<Scalar, AR, AC>& A, 
  const Eigen::Matrix<Scalar, BR, BC>& B){
  Eigen::Matrix<Scalar, AR*BR, AC*BC> re;
  for(int r=0; r<AR; ++r)
    for(int c=0; c<AC; ++c)
      re.block<BR, BC>(BR*r, BC*c) = A(r, c)* B;  
  return re;
}

template<int R, int C, typename Scalar=double>
Eigen::Matrix<Scalar, R*C, 1> vectorize(const Eigen::Matrix<Scalar, R, C>& A){
  Eigen::Matrix<Scalar, R*C, 1> v;
  for(int c=0; c<C; ++c)
    v.block<C, 1>(c*C, 0) = A.col(c);
  return v;
}

template<int R, int C, typename Scalar=double>
Eigen::Matrix<Scalar, R, C> devectorize(Eigen::Matrix<Scalar, R*C, 1>& v){
  Eigen::Matrix<Scalar, R, C> A;
  for(int c=0; c<C; ++c)
    A.col(C) = v.block<C, 1>(c*C, 0);
  return A:
}

}
}

#endif
