#ifndef DBSCAN_HPP_
#define DBSCAN_HPP_

#include "ll.hpp"

enum PointState {
  InitCluster = 0,
  Undefined = -1,
  Noise = -2,
};

// require: NeighborFunctor:: [Point] -> size_t -> [size_t]
template <typename Point, typename NeighborFunctor>
std::vector<std::vector<std::size_t>>
DBSCAN(const std::vector<Point> &points, const NeighborFunctor &neighbor_fun,
       std::size_t minimal_neighbors) {
  std::vector<int> states(points.size(), Undefined);

  int c{InitCluster}; // cluster id

  for (std::size_t i = 0; i < points.size(); ++i) {
    if (states[i] != Undefined)
      continue;

    const auto nids = neighbor_fun(points, i);
    if (nids.size() < minimal_neighbors) {
      states[i] = Noise;
      continue;
    }

    // found a cluster
    states[i] = c;
    std::queue<std::size_t> seeds;
    for (auto ni : nids)
      seeds.push(ni);

    while (!seeds.empty()) {
      std::size_t j = seeds.front();
      seeds.pop();

      if (states[j] == Noise)
        states[j] = c; // noise as border
      if (states[j] != Undefined)
        continue;

      states[j] = c;
      const auto nnids = neighbor_fun(points, j);
      if (nnids.size() >= minimal_neighbors) // core point
        for (auto nj : nnids)
          seeds.push(nj);
    }

    ++c;
  }

  return ll::group_by(ll::sort_indices(states),
                      [&states](std::size_t i) { return states[i]; });
}

#endif