#pragma once

#include "ll.hpp"

// define below macro to use them before #include
// #define LL_USE_OPENCV
#define LL_USE_EIGEN

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
#include <Eigen/Dense>

namespace ll {
namespace tp {

template <int AR, int AC, int BR, int BC, typename Scalar = double>
Eigen::Matrix<Scalar, AR * BR, AC * BC> kronecker(
    const Eigen::Matrix<Scalar, AR, AC>& A,
    const Eigen::Matrix<Scalar, BR, BC>& B) {
  Eigen::Matrix<Scalar, AR * BR, AC * BC> re;
  for (int r = 0; r < AR; ++r)
    for (int c = 0; c < AC; ++c) re.block<BR, BC>(BR * r, BC * c) = A(r, c) * B;
  return re;
}

template <int R, int C, typename Scalar = double>
Eigen::Matrix<Scalar, R * C, 1> vectorize(
    const Eigen::Matrix<Scalar, R, C>& A) {
  Eigen::Matrix<Scalar, R * C, 1> v;
  // for (int c = 0; c < C; ++c) v.block<C, 1>(c * C, 0) = A.col(c);
  for (int c = 0; c < C; ++c) v.block(c * C, 0, C, 1) = A.col(c);
  return v;
}

template <int R, int C, typename Scalar = double>
Eigen::Matrix<Scalar, R, C> devectorize(Eigen::Matrix<Scalar, R * C, 1>& v) {
  Eigen::Matrix<Scalar, R, C> A;
  for (int c = 0; c < C; ++c) A.col(C) = v.block<C, 1>(c * C, 0);
  return A;
}

template <typename T>
using mat3x3 = Eigen::Matrix<T, 3, 3>;
template <typename T>
using vec3 = Eigen::Matrix<T, 3, 1>;

template <typename T>
mat3x3<T> asSkewSymmetricMatrix(const vec3<T>& v) {
  mat3x3<T> ssm = mat3x3<T>::Zero();
  ssm(0, 1) = -v(2);
  ssm(1, 0) = v(2);
  ssm(0, 2) = v(1);
  ssm(2, 0) = -v(1);
  ssm(1, 2) = -v(0);
  ssm(2, 1) = v(0);

  return ssm;
}

// Eigen::Matrix3d exp(const Eigen::Vector3d& s) {
//   //! refer to g2o/se3quat.h
//   double theta = s.norm();

//   Eigen::Matrix3d Omega = asSkewSymmetricMatrix(s);

//   if (theta < 0.00001)
//     return Eigen::Matrix3d::Identity() + Omega + Omega * Omega;
//   else {
//     return Eigen::Matrix3d::Identity() + sin(theta) / theta * Omega +
//            (1 - cos(theta)) / (theta * theta) * (Omega * Omega);
//   }
// }

// Eigen::Vector3d log(const Eigen::Matrix3d& R) {
//   Eigen::Vector3d dR;
//   dR(0) = R(2, 1) - R(1, 2);
//   dR(1) = R(0, 2) - R(2, 0);
//   dR(2) = R(1, 0) - R(0, 1);

//   double d = .5 * (R(0, 0) + R(1, 1) + R(2, 2) - 1.);
//   if (d > 0.99999) return 0.5 * dR;

//   double theta = std::acos(d);
//   return theta / (2 * std::sqrt(1 - d * d)) * dR;
// }

template <typename T>
Eigen::Matrix<T, 6, 1> log(const mat3x3<T>& R, const vec3<T>& t) {
  double d = .5 * (R(0, 0) + R(1, 1) + R(2, 2) - 1.);

  vec3<T> dR, omega, upsilon;
  dR(0) = R(2, 1) - R(1, 2);
  dR(1) = R(0, 2) - R(2, 0);
  dR(2) = R(1, 0) - R(0, 1);

  mat3x3<T> V_inv;

  if (d > 0.99999) {
    omega = .5 * dR;
    mat3x3<T> Omega = asSkewSymmetricMatrix(omega);
    V_inv = mat3x3<T>::Identity() - 0.5 * Omega + 1. / 12. * (Omega * Omega);
  } else {
    double theta = std::acos(d);
    omega = theta / (2 * std::sqrt(1 - d * d)) * dR;
    mat3x3<T> Omega = asSkewSymmetricMatrix(omega);
    V_inv = (mat3x3<T>::Identity() - 0.5 * Omega +
             (1 - theta / (2 * tan(theta / 2))) / (theta * theta) *
                 (Omega * Omega));
  }

  upsilon = V_inv * t;

  Eigen::Matrix<T, 6, 1> re;
  // re.block<3, 1>(0, 0) = omega;
  // re.block<3, 1>(3, 0) = upsilon;

  re.block(0, 0, 3, 1) = omega;
  re.block(3, 0, 3, 1) = upsilon;

  return re;
}
}
}

#endif
