
#include <Eigen/Core>
#include <flann/flann.h>
#include "ll.hpp"
#include "base_type.h"

namespace cloud_mapping {
namespace algo_ul {

// todo:
template <typename T, int DIM>
class FlannIndex {
  using Index = flann::Index<L2_Simple<T>>;

 public:
  using Point = Eigen::Matrix<T, DIM, 1>;
  using Points = Eigen::vector<Point>;
  struct QueryRe {
    std::vector<std::vector<int>> indices;
    std::vector<std::vector<T>> dists;
  };

  void Build(const Points& points, int leafsize) {
    // LOG_IF(WARN, !index_) << "rebuild flann index.";
    if (index_) LOG_WARN << "rebuild flann index.";
    index_ = std::make_shared<Index>(AsMatrix(points),
                                     flann::KDTreeSingleIndexParams(leafsize));
    index_->buildIndex();
  }

  QueryRe QueryRadius(const Points& points, T r) const {
    QueryRe qr;
    qr.indices.reserve(points.size());
    qr.dists.reserve(points.size());
    index_->radiusSearch(AsMatrix(points), qr.indices, qr.dists, r * r,
                         sparam_);
    return qr;
  }

  QueryRe QueryNearestN(const Points& points, std::size_t n) const {
    QueryRe qr;
    qr.indices.reserve(points.size());
    qr.dists.reserve(points.size());
    index_->knnSearch(AsMatrix(points), qr.indices, qr.dists, n, sparam_);
    return qr;
  }

  Point GetPoint(int i) const { return Eigen::Map<Point>(index_->getPoint(i)); }

  const Index& GetIndex() const { return *index_; }

 private:
  std::shared_ptr<Index> index_{nullptr};
  flann::SearchParams sparam_{-1, 1e-6f};

  inline flann::Matrix<T> AsMatrix(const Points& points) const {
    return flann::Matrix<T>(const_cast<T*>(&points[0][0]), points.size(), DIM);
  }
};

// auto deduce
template <typename T, int DIM>
FlannIndex<T, DIM> BuildFlannIndex(
    const Eigen::vector<Eigen::Matrix<T, DIM, 1>>& points, int leafsize = 15) {
  CHECK(!points.empty());
  FlannIndex<T, DIM> fi;
  fi.Build(points, leafsize);
  return fi;
}

}  // namespace algo_ul
}  // namespace cloud_mapping
