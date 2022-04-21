#include "dbscan.hpp"
#include "flann_search.hpp"

template <typename Point>
std::vector<std::vector<std::size_t>>
DBSCAN_L2(const std::vector<Point> &points, double radius,
          int minimal_neighbors) {
  auto neighbor_func = [fi = BuildFlannIndex(points), radius](
                           const std::vector<Point> &points, std::size_t i) {
    return fi.QueryRadius(std::vector<Point>{points[i]}, radius)
        .indices.front();
  };

  return DBSCAN(points, neighbor_func, minimal_neighbors);
}
