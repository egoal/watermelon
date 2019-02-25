#include "ll.hpp"

namespace ll {
namespace hl {

class environment;

// variant holds any kind of value
class cell {
public:
  using Proc = std::function<cell(const std::vector<cell>&)>;

  enum class type {
    sym,
    num,
    str,
    list,
    proc,
    lambda,
  };

  cell(type t = type::sym) : tp(t) {}
  cell(type t, const std::string& val) : tp(t), val(val) {}
  cell(Proc p) : tp(type::proc), proc(p) {}

  // outer usage
  int i() const { return std::stoi(val); }
  double d() const { return std::stod(val); }
  std::string s() const { return val; }
  bool b() const {
    try {
      return !(val == "false" || val == "nil" || std::stoi(val) == 0);
    } catch (std::exception& e) {
      return true;
    }
  }

  bool operator==(const cell& other) const {
    return tp == other.tp && val == other.val;
  }

  cell operator()(const std::vector<cell>& params);

  cell operator()(const std::vector<std::string>& args) {
    std::vector<cell> params(args.size());
    std::transform(args.begin(), args.end(), params.begin(), atom);
    return operator()(params);
  }

  cell operator()(const std::string& argsline) {
    return operator()(ll::string_split(argsline, ' '));
  }

  std::string to_string() const {
    switch (tp) {
      case type::list:
        return std::accumulate(list.begin(), list.end(), std::string("["),
                   [](const std::string& sum, const cell& c) {
                     return sum + c.to_string() + ", ";
                   }) +
               "]";
      case type::lambda: return "<lambda>";
      case type::proc: return "<process>";
      case type::str: return "\'" + val + "\'";
      default: return val;
    }
  }

  type tp;
  std::string val;
  std::vector<cell> list;
  Proc proc;
  environment* env{nullptr};

  // consts
  const static cell F;
  const static cell T;
  const static cell N;

  // convert to number or symbol
  static cell atom(const std::string& token) {
    if (std::isdigit(token[0]) || (token[0] == '-' && std::isdigit(token[1])))
      return cell(cell::type::num, token);
    if (token[0] == '\'' || token[0] == '\"')
      return cell(cell::type::str, token.substr(1, token.size() - 2));
    return cell(cell::type::sym, token);
  }
};

const cell cell::F(cell::type::sym, "false");
const cell cell::T(cell::type::sym, "true");
const cell cell::N(cell::type::sym, "nil");

using cells = std::vector<cell>;

// basic proc functions
// algebraic
cell proc_add(const cells& params) {
  auto sum = std::accumulate(params.begin() + 1, params.end(),
      std::stod(params.front().val),
      [](double sum, const cell& c) { return sum + std::stod(c.val); });
  return cell(cell::type::num, std::to_string(sum));
}

cell proc_sub(const cells& params) {
  auto sum = std::accumulate(params.begin() + 1, params.end(),
      std::stod(params.front().val),
      [](double sum, const cell& c) { return sum - std::stod(c.val); });
  return cell(cell::type::num, std::to_string(sum));
}

cell proc_mul(const cells& params) {
  auto sum = std::accumulate(params.begin() + 1, params.end(),
      std::stod(params.front().val),
      [](double sum, const cell& c) { return sum * std::stod(c.val); });
  return cell(cell::type::num, std::to_string(sum));
}

cell proc_div(const cells& params) {
  auto sum = std::accumulate(params.begin() + 1, params.end(),
      std::stod(params.front().val),
      [](double sum, const cell& c) { return sum / std::stod(c.val); });
  return cell(cell::type::num, std::to_string(sum));
}

cell proc_mod(const cells& params) {
  auto sum = std::accumulate(params.begin() + 1, params.end(),
      params.front().i(), [](int sum, const cell& c) { return sum % c.i(); });
  return cell(cell::type::num, std::to_string(sum));
}

cell proc_comp(
    const cells& params, std::function<bool(const cell&, const cell&)> func) {
  return std::is_sorted(params.begin(), params.end(), func) ? cell::T : cell::F;
}

// assists
cell proc_print(const cells& params) {
  for (auto& c : params) std::cout << c.to_string() << " ";
  std::cout << "\n";
  return cell::N;
}

// dictionary associates symbols with cells as well as chain to outer space
class environment {
public:
  using map = std::unordered_map<std::string, cell>;

  environment() {}
  environment(const std::vector<cell>& params, const std::vector<cell>& args,
      environment* o)
      : outer(o) {
    auto it = args.begin();
    for (auto p : params) {
      dic[p.val] = *it++;
    }
  }

  // repl
  void start_repl(const std::string& prompt = ">> ") {
    std::string line;
    while (true) {
      std::cout << prompt;
      std::getline(std::cin, line);
      if (line == "exit" || line == "quit") break;
      std::cout << eval(read(line)).to_string() << "\n";
    }
  }

  void do_line(const std::string& line) { eval(read(line)); }

  void do_file(const std::string& filename) {
    std::ifstream fin(filename);
    std::string jointline = "(run ";
    {
      // remove comments
      std::string line;
      while (std::getline(fin, line)) {
        line = ll::string_strip(line);
        jointline += line.substr(0, line.find(';'));
      }
    }

    jointline += " )";

    do_line(jointline);
  }

  void print() {
    for (auto& pr : dic) {
      std::cout << pr.first << ": " << pr.second.to_string() << "\n";
    }
  }

  cell& operator[](const std::string& key) { return dic[key]; }

  cell get(const std::string& key) const {
    auto iter = dic.find(key);
    return iter == dic.end() ? cell::N : iter->second;
  }

  cell& find(const std::string& key) {
    if (dic.find(key) != dic.end()) return dic[key];
    if (outer) return outer->find(key);
    // not exists
    LL_LOG << "undefined symbol: " << key << "\n";
    exit(1);
  }

  cell& find_add(const std::string& key) {
    auto env = this;
    while (env) {
      if (env->dic.find(key) != dic.end()) return env->dic[key];
      env = env->outer;
    }

    return dic[key];  // no found, creat a value in current env
  }

  // globla behaviours
  void add_globals() {
    dic["nil"] = cell::N;
    dic["false"] = cell::F;
    dic["true"] = cell::T;

    dic["+"] = cell(proc_add);
    dic["-"] = cell(proc_sub);
    dic["*"] = cell(proc_mul);
    dic["/"] = cell(proc_div);
    dic["%"] = cell(proc_mod);

    dic["="] = cell(std::bind(proc_comp, std::placeholders::_1,
        [](const cell& c1, const cell& c2) -> bool {
          return c1.d() == c2.d();
        }));
    dic["<"] = cell(std::bind(proc_comp, std::placeholders::_1,
        [](const cell& c1, const cell& c2) -> bool {
          return c1.d() < c2.d();
        }));
    dic["<="] = cell(std::bind(proc_comp, std::placeholders::_1,
        [](const cell& c1, const cell& c2) -> bool {
          return c1.d() <= c2.d();
        }));
    dic[">"] = cell(std::bind(proc_comp, std::placeholders::_1,
        [](const cell& c1, const cell& c2) -> bool {
          return c1.d() > c2.d();
        }));
    dic[">="] = cell(std::bind(proc_comp, std::placeholders::_1,
        [](const cell& c1, const cell& c2) -> bool {
          return c1.d() >= c2.d();
        }));

    dic["print"] = cell(proc_print);
  }

  cell eval(cell x) {
    if (x.tp == cell::type::sym) return find(x.val);
    if (x.tp == cell::type::num || x.tp == cell::type::str) return x;

    if (x.list.empty()) return cell::N;

    // then x is a list
    if (x.list[0].tp == cell::type::sym) {
      auto tag = x.list[0].val;
      if (tag == "if")
        return eval(eval(x.list[1]).b()
                        ? x.list[2]
                        : (x.list.size() < 4 ? cell::N : x.list[3]));
      if (tag == "lambda") {
        //^ defind a lambda func (lambda (args...) exp)
        x.tp = cell::type::lambda;
        x.env = this;
        return x;
      }
      if (tag == "let")  // let varname var
        return find_add(x.list[1].val) = eval(x.list[2]);
      if (tag == "fn") {  // fn funname args exps
        cell f(cell::type::lambda);
        f.env = this;
        f.list.push_back(cell(cell::type::sym, "lambda"));
        f.list.insert(f.list.end(), x.list.begin() + 2, x.list.end());
        return find_add(x.list[1].val) = f;
      }
      if (x.list[0].val == "run") {
        //^ execute
        for (std::size_t i = 1; i < x.list.size() - 1; ++i) eval(x.list[i]);
        return eval(x.list.back());
      }
    }

    // not the standard behaviour, (proc exps...)
    cell proc(eval(x.list[0]));
    std::vector<cell> exps;
    std::transform(x.list.begin() + 1, x.list.end(), std::back_inserter(exps),
        [&](cell& c) { return eval(c); });

    return proc(exps);
  }

private:
  map dic;
  environment* outer{nullptr};

  cell read(const std::string& line) {
    auto tokens = tokenize(line);
    return read_from(tokens);
  }

  std::list<std::string> tokenize(const std::string& str) {
    std::list<std::string> tokens;
    std::size_t len = str.size();
    std::size_t i = 0;
    while (i < len) {
      while (str[i] == ' ') ++i;
      if (i >= len) break;
      if (str[i] == '(') {
        tokens.push_back("(");
        ++i;
      } else if (str[i] == ')') {
        tokens.push_back(")");
        ++i;
      } else {
        std::size_t s = i;
        while (i < len && str[i] != ' ' && str[i] != '(' && str[i] != ')') ++i;
        tokens.push_back(str.substr(s, i - s));
      }
    }
    return tokens;
  }

  // create tree
  cell read_from(std::list<std::string>& tokens) {
    const auto token = tokens.front();
    tokens.pop_front();
    if (token == "(") {
      cell c(cell::type::list);
      while (tokens.front() != ")") c.list.push_back(read_from(tokens));
      tokens.pop_front();
      return c;
    } else
      return cell::atom(token);
  }
};

cell cell::operator()(const std::vector<cell>& params) {
  if (tp == type::proc) return proc(params);

  if (tp == type::lambda) {
    auto e = std::unique_ptr<environment>(
        new environment(list[1].list, params, env));
    return e->eval(list[2]);
  }

  LL_LOG << "undefined process: " << val << "\n";
  exit(1);
}

environment do_file(const std::string& file) {
  environment e;
  e.add_globals();
  e.do_file(file);
  return e;
}

environment do_line(const std::string& line) {
  environment e;
  e.add_globals();
  e.do_line(line);
  return e;
}
}
}
