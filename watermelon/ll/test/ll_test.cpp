#include "ll.hpp"
#include "llda.hpp"
#include <gtest/gtest.h>

TEST(llda, MeanAndVariance) {
  auto [mu, sigma] = ll::da::mean_and_variance(ll::range(0., 1., 0.1).vec());

  EXPECT_DOUBLE_EQ(mu, .5);
  EXPECT_DOUBLE_EQ(sigma, .1);
}
