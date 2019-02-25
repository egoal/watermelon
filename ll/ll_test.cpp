// simple test

#include "ll.hpp"
#include "llda.hpp"
#include "llhl.hpp"
#include "llui.hpp"

#define NUMBER_EQUAL(a, b) \
  if (LL_ABS(a - b) > 1e-8) LL_ABORT("equal test failed: ")

void testConfig() {
  ll::ConfigParser cp("config");

  cp.print(std::cout);
  std::cout << cp.getNumber<int>("find-kps-num") << std::endl;
  auto vs = cp.getNumberList("frames-of-interested");
  for (auto& v : vs) std::cout << v << ", ";
  std::cout << std::endl;
}

void testRange() {
  for (auto& v : ll::range(1., 0., -.2)) std::cout << v << " ";
  std::cout << "\n";
}

void testMean() {
  std::vector<double> v{1, 2, 3, 4, 5, 6, 7, 8};
  ll::print(ll::da::mean_and_variance(v));
}

void testUnique() {
  int a[]{5, 3, 6, 1, 2, 1, 2, 3, 4, 4, 3, 2, 0, 1};
  std::vector<int> v(a, a + sizeof(a) / sizeof(int));
  // std::function<bool (int, int)> f   =   [](int i1, int i2){ return i1==i2;
  // };
  auto it = ll::unique(v.begin(), v.end());
  std::for_each(v.begin(), it, [](int i) { std::cout << i << ", "; });
}

void testFp() {
  std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};

  auto echo = [](int i) { std::cout << i << ", "; };
  ll::for_each(vec, echo);
  std::cout << std::endl;

  auto addone = [](int i) { return i + 1; };
  ll::map(vec, addone);

  ll::for_each(vec, echo);
  std::cout << std::endl;

  ll::zip(vec, vec, [](int i, int j) { return i * j; });

  ll::for_each(vec, echo);
  std::cout << std::endl;

  ll::filter(vec, [](int i) { return i % 2 == 0; });

  ll::for_each(vec, echo);
  std::cout << std::endl;

  std::vector<int> vec2{1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> vec3 = ll::range(10);
  ll::filter(ll::zip(ll::map(vec, [](int i) { return i + 1; }), vec3,
                 [](int i, int j) { return i + j; }),
      [](int i) { return i % 2 == 0; });

  ll::for_each(vec2, echo);
  std::cout << std::endl;
  ll::for_each(vec3, echo);
  std::cout << std::endl;
}

void testMatch() {
  std::vector<int> vec0{0, 1, 2, 4, 6, 8};
  auto vec1 = ll::range(10);

  auto mths = ll::da::match_bf(vec0.size(), vec1.size(),
      [&](int i, int j) { return (vec0[i] - vec1[j]) * (vec0[i] - vec1[j]); });

  for (auto& m : mths) {
    int id0, id1;
    double dis;
    std::tie(id0, id1, dis) = m;
    std::cout << id0 << " -> " << id1 << ": " << dis << "\n";
  }
}

void testPlot() {
  auto xs = ll::range(0., 10., .1);
  std::vector<double> ys;
  std::transform(xs.begin(), xs.end(), std::back_inserter(ys),
      [](double x) { return std::sin(x); });

  ll::ui::board b(200, 60);
  ll::ui::plot(b, xs, ys, 'o');

  std::transform(
      xs.begin(), xs.end(), ys.begin(), [](double x) { return std::cos(x); });

  ll::ui::plot(b, xs, ys, '+').print();
}

void testHL() {
  // auto e = ll::hl::do_file("l");
  auto e = ll::hl::do_line(
      "(run (let distributed 1) (let dynamic-scaling 0) (let max-iterations "
      "10) "
      " (let max-inner-iterations 10)    (if dynamic-scaling         (run"
      "(let static-extra-rho-kf 100.) (let static-extra-rho-mp 100.))"
      "(run    (let dynamic-increase-kf 50.) (let dynamic-increase-mp 50.)))"
      " (fn get-inner-iterations (i)  (if (> i 10) 15 25)))");

  std::cout << std::boolalpha << e.get("distributed").b() << "\n";
  auto f = e.get("get-inner-iterations");
  for (int i = 0; i < 20; ++i) std::cout << f(std::to_string(i)).i() << ", ";
  std::cout << std::endl;

  e.start_repl("test val>");
}

int main(int, char** argv) {
  testHL();

  return 0;
}
