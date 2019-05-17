#pragma once

#include "ll.hpp"
#include "llda.hpp"

namespace ll {
namespace ui {

struct mulchar {
  mulchar(char c, int num) : c_(c), count_(num) {}
  char c_;
  int count_;
};
std::ostream& operator<<(std::ostream& os, const mulchar& dc) {
  LL_REPEAT(dc.count_) os << dc.c_;
  return os;
}

class canvas {
public:
  canvas(int r, int c) : rows(r), cols(c) {
    dots = new char[rows * cols];
    fill(' ');
  }
  ~canvas() { delete[] dots; }

  // this allows: canvas[row][col]
  char* operator[](int r) const { return dots + r * cols; }

  void fill(char c) {
    // std::memset(dots, c, sizeof(char));
    std::fill(dots, dots + rows * cols, c);
  }
  void fill(int rbeg, int rend, int cbeg, int cend, char ch) {
    for (int r = rbeg; r < rend; ++r)
      for (int c = cbeg; c < cend; ++c) set(r, c, ch);
  }
  void set(int r, int c, char ch) { dots[r * cols + c] = ch; }

  void print(std::ostream& os = std::cout) const {
    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) os << dots[r * cols + c];
      os << "\n";
    }
  }

  int rows, cols;

private:
  char* dots;
};

class coordinate {
public:
  coordinate(canvas& _can, double xmin, double xmax, double ymin, double ymax)
      : can(_can), xmin_(xmin), xmax_(xmax), ymin_(ymin), ymax_(ymax) {
    can.fill(0, 1, 0, can.cols, '-');
    // can.fill(can.rows - 1, can.rows, 0, can.cols, '-');
    can.fill(0, can.rows, 0, 1, '|');
    // can.fill(0, can.rows, can.cols-1, can.cols, '|');
    can.set(0, 0, '+');
  }

  char& at(double x, double y) const {
    return can[1 + int(std::round((lambda(x, xmin_, xmax_) * (can.rows - 2))))]
              [1 + int(std::round((lambda(y, ymin_, ymax_) * (can.cols - 2))))];
  }

private:
  canvas& can;
  double xmin_, xmax_, ymin_, ymax_;

  double lambda(double val, double min, double max) const {
    return ll::clamp((val - min) / (max - min), 0., 1.);
  }
};

template <typename T = double>
canvas& plot(canvas& can, const std::vector<T>& xs, const std::vector<T>& ys,
    const char ch = '*') {
  assert(xs.size() == ys.size());
  auto prx = std::minmax_element(xs.begin(), xs.end());
  auto pry = std::minmax_element(ys.begin(), ys.end());
  coordinate coord(can, *prx.first, *prx.second, *pry.first, *pry.second);

  for (std::size_t i = 0; i < xs.size(); ++i) {
    coord.at(xs[i], ys[i]) = ch;
  }

  return can;
}
}
}