
#pragma once

#include "ll.hpp"

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseCore>

namespace ll {

template <int ROW, int COL>
class SparseBlockMatrix {
  LL_BAN_COPY(SparseBlockMatrix);

public:
  using Block = Eigen::Matrix<double, ROW, COL>;

  SparseBlockMatrix(int rows, int cols) : rows_(rows), cols_(cols) {
    LL_ASSERT(rows_ > 0 && cols_ > 0 && "bad allocate.");
  }

  //! auto allocate
  Block& operator()(int r, int c) {
    int id = _id(r, c);
    auto iter = blocks_.find(id);
    if (iter == blocks_.end()) blocks_[id] = Block::Zero();

    return blocks_[id];
  }

  //! no error check
  const Block& operator()(int r, int c) const { return blocks_[_id(r, c)]; }

  void fill(Eigen::SparseMatrix<double>& smat) const {
    smat.resize(ROW * rows_, COL * cols_);
    smat.reserve(blocks_.size() * ROW * COL);

    // fill
    int r, c;
    for (auto& pr : blocks_) {
      std::tie(r, c) = _rc(pr.first);
      r *= ROW;
      c *= COL;
      for (int rr = 0; rr < ROW; ++rr)
        for (int cc = 0; cc < COL; ++cc)
          smat.insert(r + rr, c + cc) = pr.second(rr, cc);
    }

    smat.makeCompressed();
  }

  void fill(Eigen::MatrixXd& mat) const {
    mat.resize(ROW * rows_, COL * cols_);
    mat.fill(0.);

    int r, c;
    for (auto& pr : blocks_) {
      std::tie(r, c) = _rc(pr.first);
      r *= ROW;
      c *= COL;
      for (int rr = 0; rr < ROW; ++rr)
        for (int cc = 0; cc < COL; ++cc)
          mat(r + rr, c + cc) = pr.second(rr, cc);
    }
  }

  //! debug
  void write(std::ostream& os) {
    int i, j;
    std::vector<int> ids;
    ids.reserve(blocks_.size());
    for (auto& pr : blocks_) ids.push_back(pr.first);
    std::sort(ids.begin(), ids.end());
    for (auto id : ids) {
      std::tie(i, j) = _rc(id);
      os << i << ", " << j << ") ";
      for (int r = 0; r < ROW; ++r)
        for (int c = 0; c < COL; ++c) os << blocks_[id](r, c) << ", ";
      os << "\n";
    }
  }

private:
  int rows_, cols_;
  std::unordered_map<int, Block> blocks_;

  // fixme: overflow error
  inline int _id(int r, int c) const {
    LL_ASSERT(r >= 0 && r < rows_ && c >= 0 && c < cols_);
    return r * cols_ + c;
  }
  inline std::tuple<int, int> _rc(int id) const {
    return std::make_tuple(id / cols_, id % cols_);
  }
};

inline void sbm_test() {
  return;
  SparseBlockMatrix<2, 2> sbm(4, 4);
  sbm(0, 0) = Eigen::Matrix<double, 2, 2>::Ones();
  sbm(1, 1) = Eigen::Matrix<double, 2, 2>::Ones() * 2;
  sbm(3, 2) = Eigen::Matrix<double, 2, 2>::Ones() * 4;
  sbm(2, 3) << 1, 2, 3, 4;
  sbm(3, 3) << 5, 6, 7, 8;

  Eigen::SparseMatrix<double> sm;
  sbm.fill(sm);
  Eigen::MatrixXd m;
  sbm.fill(m);
  std::cout << sm << "\n" << m;
  std::cout << "\n--------------------------" << std::endl;
}
}
