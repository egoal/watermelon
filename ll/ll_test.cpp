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
  std::vector<double> v = ll::range(0., 1., 0.1);
  ll::print(ll::da::mean_and_variance(v), std::cout);
  std::cout << "\n";

  auto vals = ll::da::percent_quantile(v, {0., 0.4, 0.5, 0.6, 1.});
  for (auto v : vals) std::cout << v << ", ";
  std::cout << std::endl;
}

void testUnique() {
  int a[]{5, 3, 6, 1, 2, 1, 2, 3, 4, 4, 3, 2, 0, 1};
  std::vector<int> v(a, a + sizeof(a) / sizeof(int));
  // std::function<bool (int, int)> f   =   [](int i1, int i2){ return i1==i2;
  // };
  auto it = ll::unique(v.begin(), v.end());
  std::for_each(v.begin(), it, [](int i) { std::cout << i << ", "; });
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

  ll::ui::canvas c(100, 80);
  ll::ui::plot(c, xs, ys).print();
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

// ||header||body||
// ||length| key length| key|| value||

constexpr int MAX_KEY_LENGTH = 64;

class OutBundle {
public:
  OutBundle(const std::string& filename)
      : os_(filename, std::ios::out | std::ios::binary) {}

  template <typename T>
  void putValue(const std::string& key, T value) {
    writeHeader(key, sizeof(T));
    writeValue(value);
  }

private:
  std::ofstream os_;

  template <typename T>
  void writeValue(T t, int len = sizeof(T)) {
    os_.write(reinterpret_cast<const char*>(&t), len);
  }

  void writeHeader(const std::string& key, int valuelen) {
    writeValue(sizeof(int) + sizeof(int) + key.size() + valuelen);
    writeValue(key.size());
    os_.write(key.c_str(), key.size());
  }
};

class InBundle {
public:
  InBundle(const std::string& filename)
      : is_(filename, std::ios::in | std::ios::binary) {}

  template <typename T>
  T getValue(const std::string& key, T defval) {
    readHeader();
    readValue(defval);
    return defval;
  }

private:
  std::ifstream is_;

  template <typename T>
  void readValue(T& t, int len = sizeof(T)) {
    is_.read(reinterpret_cast<char*>(&t), len);
  }

  void readHeader() {
    static char buff[MAX_KEY_LENGTH];
    int totallen, keylen;
    std::string key;
    readValue(totallen);
    readValue(keylen);
    is_.read(buff, keylen);
    buff[keylen] = '\0';
    key = buff;
    std::cout << totallen << ", " << keylen << ", " << key << "\n";
    // todo:
  }
};

int main(int, char** argv) {
  {
    OutBundle ob("a");
    ob.putValue("rotation", 100);
  }

  {
    InBundle ib("a");
    std::cout << ib.getValue("rotation", 0) << std::endl;
  }

  return 0;
}
