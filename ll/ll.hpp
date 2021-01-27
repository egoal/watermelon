#pragma once

// std headers
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <sstream>
#include <string>

#include <array>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <algorithm>
#include <exception>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>
#include <type_traits>

#include <cassert>
#include <ctime>

// macros
#define LL_BAN_COPY(X)  \
  X(const X&) = delete; \
  X& operator=(const X&) = delete

#define LL_ABORT(...) throw std::runtime_error(__VA_ARGS__)
#define LL_ASSERT(...) \
  if (!(__VA_ARGS__)) LL_ABORT(#__VA_ARGS__)

#define __FILENAME__ ll::__current_filename(__FILE__)
#define LL_LOG std::cerr << "[" << __FILENAME__ << ": " << __LINE__ << "] "
#define LL_LOG_F(str, ...) { std::sprintf(ll::__logchar_mem__, str, __VA_ARGS__); LL_LOG << ll::__logchar_mem__; }

#define LL_NOTNULL(x)                           \
  {                                             \
    if (!x) LL_ABORT("null check failed: " #x); \
  }                                             \
  x

#define LL_ABS(x) ((x) > 0 ? (x) : (-x))

#define LL_TOFILE(f, ...)                                 \
  {                                                       \
    std::ofstream fout(f, std::ios::out | std::ios::app); \
    ll::CommaOS cos(fout);                                \
    cos, __VA_ARGS__;                                     \
  }

#define LL_REPEAT(n) for (int __i__ = 0; __i__ < n; ++__i__)

namespace ll {
inline std::string __current_filename(const std::string& file) {
	auto pos = file.rfind('\\');
	return file.substr(pos == std::string::npos ? 0 : pos + 1);
}

static char __logchar_mem__[1024]; // memory for LL_LOG_F

#if __cplusplus < 201402L
//* before c++14
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

// range vector
// todo: make it a iteratable struct instead.
template <typename T>
std::vector<T> range(T e) {
	std::vector<T> vec(e);
	std::iota(vec.begin(), vec.end(), 0);
	return vec;
}
template <typename T>
std::vector<T> range(T s, T e, T d) {
	assert((e - s) / d > 0 && "invalid range call");
	std::vector<T> vec(int((e - s) / d), d);
	vec[0] = s;
	std::partial_sum(vec.begin(), vec.end(), vec.begin());
	return vec;
}

template <typename T>
T clamp(T val, T low, T high) {
	assert(low <= high && "bad arguments");
	if (val < low) return low;
	if (val > high) return high;
	return val;
}

template <typename T>
T rand(T maxval) {
	return T(std::rand() / double(RAND_MAX) * maxval);
}
template <typename T>
T randrange(T minval, T maxval) {
	return T(std::rand() / double(RAND_MAX) * (maxval - minval)) + minval;
}

//* tuple print
template <typename Tuple, std::size_t N>
struct __tuple_printer {
	static void print(const Tuple& t, std::ostream& os) {
		__tuple_printer<Tuple, N - 1>::print(t, os);
		os << ", " << std::get<N - 1>(t);
	}
};
template <typename Tuple>
struct __tuple_printer<Tuple, 1> {
	static void print(const Tuple& t, std::ostream& os) { os << std::get<0>(t); }
};
template <typename... Args>
void print(const std::tuple<Args...>& t, std::ostream& os = std::cout) {
	os << "(";
	__tuple_printer<decltype(t), sizeof...(Args)>::print(t, os);
	os << ")";
}

//* simple fp patch, we only have c++11 for now.
template<typename Collection, typename UOP>
auto max_by(UOP f, const Collection& c)-> std::pair<decltype(c.begin()), decltype(f(*c.begin()))> {
	auto largest = c.begin();
	auto s = f(*largest);

	for (auto iter = c.begin() + 1; iter != c.end(); ++iter) {
		auto s2 = f(*iter);
		if (s2 > s) {
			largest = iter;
			s = s2;
		}
	}

	return std::make_pair(largest, s);
}

template<typename Collection, typename UOP>
auto min_by(UOP f, const Collection& c)-> std::pair<decltype(c.begin()), decltype(f(*c.begin()))> {
	auto smallest = c.begin();
	auto s = f(*smallest);

	for (auto iter = c.begin() + 1; iter != c.end(); ++iter) {
		auto s2 = f(*iter);
		if (s2 < s) {
			smallest = iter;
			s = s2;
		}
	}

	return std::make_pair(smallest, s);
}

template <typename Collection, typename UOP>
auto mapf(UOP f, const Collection& c)->std::vector<decltype(f(*c.begin())) > {
	std::vector<decltype(f(*c.begin())) > v(c.size());
	std::transform(c.cbegin(), c.cend(), v.begin(), f);
	return v;
}

template <typename Collection, typename UOP>
Collection filter(UOP f, const Collection& c) {
	Collection re;
	std::copy_if(c.cbegin(), c.cend(), std::inserter(re, re.end()), f);
	return re;
}

template <typename T, typename Collection, typename BOP>
T fold(BOP f, T init, const Collection& c) {
	return std::accumulate(c.cbegin(), c.cend(), init, f);
}

template <typename T, typename Collection, typename BOP>
T reduce(BOP f, const Collection& c) {
	return std::accumulate(c.cbegin() + 1, c.cend(), *c.cbegin(), f);
}

template <typename Collection>
auto sum(const Collection& c) -> typename std::remove_reference<decltype(*c.begin())>::type {
	return std::accumulate(c.cbegin() + 1, c.cend(), *c.cbegin());
}

template <typename Collection, typename UOP>
auto sum_by(UOP f, const Collection& c) -> decltype(f(*c.begin())) {
	return sum(ll::mapf(f, c));
}

template <typename K, typename V>
K get_key(const std::pair<K, V>& pr) {
	return pr.first;
}

template <typename K, typename V>
V get_value(const std::pair<K, V>& pr) {
	return pr.second;
}

template <typename IT, typename BOP>
std::vector<std::pair<IT, IT>> group(IT beg, IT end, BOP f) {
	std::vector<std::pair<IT, IT>> res;
	if (beg == end) return res;

	auto it = beg;
	while (f(*beg, *it)) ++it;

	res.push_back(std::make_pair(beg, it));
	auto left = group(it, end, f);
	res.insert(res.end(), left.begin(), left.end());

	return res;
}

template <typename Container, typename BOP>
std::vector<Container> group(Container c, BOP f) {
	auto prs = group(c.begin(), c.end(), f);
	std::vector<Container> res;
	for (auto& pr : prs) res.emplace_back(pr.first, pr.second);
	return res;
}

template <typename IT1, typename IT2>
class Zip {
public:
	Zip(IT1 beg1, IT1 end1, IT2 beg2, IT2 end2)
		: beg1_(beg1), end1_(end1), beg2_(beg2), end2_(end2) {}

	class iterator {
	public:
		iterator(IT1 it1, IT2 it2) : pr_(it1, it2) {}

		bool operator!=(const iterator& other) const { return pr_ != other.pr_; }
		iterator operator++() {
			pr_.first++;
			pr_.second++;
			return *this;
		}

		std::pair<IT1, IT2> operator*() { return pr_; }

	private:
		std::pair<IT1, IT2> pr_;
	};

	iterator begin() { return iterator(beg1_, beg2_); }
	iterator end() { return iterator(end1_, end2_); }

	std::size_t size() const { return std::distance(beg1_, end1_); }

	IT1 beg1_, end1_;
	IT2 beg2_, end2_;
};

template <typename IT1, typename IT2>
Zip<IT1, IT2> zip(IT1 beg1, IT1 end1, IT2 beg2, IT2 end2) {
	return Zip<IT1, IT2>(beg1, end1, beg2, end2);
}

template <typename T, typename BOP, typename Container1, typename Container2>
std::vector<T> zip_with(BOP f, Container1 c1, Container2 c2) {
	LL_ASSERT(c1.size() <= c2.size());
	std::vector<T> results(c1.size());
	std::transform(c1.begin(), c1.end(), c2.begin(), results.begin(), f);
	return results;
}

template <typename IT>
class IndexedRange {
public:
	IndexedRange(IT beg, IT end) : beg_(beg), end_(end) {}

	struct value_type {
		value_type(std::size_t i, IT it) : index(i), iter(it) {}
		std::size_t index;
		IT iter;
	};

	class iterator {
	public:
		iterator(std::size_t i, IT it) : idx_(i), it_(it) {}

		bool operator!=(const iterator& other) const {
			return idx_ != other.idx_ || it_ != other.it_;
		}

		iterator operator++() {
			++idx_;
			++it_;
			return *this;
		}

		value_type operator*() { return value_type(idx_, it_); }

	private:
		std::size_t idx_;
		IT it_;
	};

	iterator begin() { return iterator(0, beg_); }
	iterator end() { return iterator(size(), end_); }

	std::size_t size() const { return std::distance(beg_, end_); }

private:
	IT beg_, end_;
};

template <typename Collection>
auto enumerate(const Collection& c)-> IndexedRange<decltype(c.begin())> {
	return IndexedRange<decltype(c.begin())>(c.begin(), c.end());
}

class CyclicIndices {
public:
	CyclicIndices(int beg, int end, int start, int loops = 1) :
		beg_(beg), end_(end), start_(start), loops_(loops) {
		LL_ASSERT(beg <= start && start < end && loops>0);
	}

	class iterator {
	public:
		iterator(int beg, int end, int i) :
			beg_(beg), end_(end), i_(i) {}

		bool operator!=(const iterator& other) {
			return i_ != other.i_ || beg_ != other.beg_ || end_ != other.end_;
		}
		iterator operator++() {
			i_++;
			return *this;
		}
		int operator*() { return (i_ - beg_) % (end_ - beg_) + beg_; }

	private:
		int beg_, end_, i_;
	};

	iterator begin() { return iterator(beg_, end_, start_); }
	iterator end() { return iterator(beg_, end_, start_ + size()); }

	std::size_t size() const { return loops_ * (end_ - beg_); }

private:
	int beg_, end_, start_, loops_;
};

// [from, to) [from, to)...
inline CyclicIndices cyclic_range(int from, int to, int start, int loops) {
	return CyclicIndices(from, to, start, loops);
}

//* string function, dont want to relate to boost
template <typename UOP>
std::vector<std::string> string_split(const std::string& str, UOP uop) {
	std::vector<std::string> vecStrs;
	int s(0), i(0);
	while (i < static_cast<int>(str.size())) {
		if (uop(str[i])) {
			if (s < i) vecStrs.push_back(str.substr(s, i - s));
			s = i + 1;
		}
		++i;
	}
	if (s < i) vecStrs.push_back(str.substr(s, i));

	return vecStrs;
}
template <>
inline std::vector<std::string> string_split<char>(
	const std::string& str, char c) {
	return string_split(str, [&](char tc) { return tc == c; });
}
template <>
inline std::vector<std::string> string_split<std::string>(
	const std::string& str, std::string s) {
	return string_split(
		str, [&](char c) { return s.find(c) != std::string::npos; });
}
template <>
inline std::vector<std::string> string_split<const char*>(
	const std::string& str, const char* ps) {
	return string_split(str, std::string(ps));
}

template<typename Collection>
std::string string_join(const Collection& c, const std::string& split = " ") {
	std::stringstream ss;
	auto iter = c.begin();
	ss << (*iter);
	for (++iter; iter != c.end(); ++iter) ss << split << (*iter);
	return ss.str();
}

inline std::string& string_strip(std::string& str) {
	str.erase(str.begin(), std::find_if(str.begin(), str.end(),
		[](char c) { return !std::isspace(c); }));
	str.erase(std::find_if(str.rbegin(), str.rend(),
		[](char c) { return !std::isspace(c); })
		.base(),
		str.end());
	return str;
}

/*file output*/
// comma to stream, can be help to macro define
class CommaOS {
public:
	CommaOS(std::ostream& os = std::cout, const std::string& deli = "")
		: os_(os), delimiter_(deli) {}

	template <typename T>
	CommaOS& operator,(T t) {
		os_ << t << delimiter_;
		return *this;
	}

private:
	std::ostream& os_;
	std::string delimiter_;
};

/* simple time counter */
class TimeCounter {
public:
	TimeCounter(std::function<void(int)> ontoc = nullptr) : ontoc_(ontoc) {
		reset();
	}
	~TimeCounter() { show(); }

	void reset() { start_ = std::chrono::high_resolution_clock::now(); }
	void show() const {
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - start_)
			.count();
		if (ontoc_)
			ontoc_(ms);
		else
			LL_LOG << ms << " ms passed.\n";
	}

private:
	decltype(std::chrono::high_resolution_clock::now()) start_;
	std::function<void(int)> ontoc_;
};

/* file config*/
// parser for my style
class ConfigParser {
public:
	ConfigParser(const std::string& filename) { reload(filename); }
	LL_BAN_COPY(ConfigParser);

	void reload(const std::string& filename) {
		std::ifstream fin(filename.c_str());
		assert(fin.is_open());
		std::string line, key, e, val;
		while (std::getline(fin, line)) {
			line = line.substr(0, line.find(';'));
			if (line.empty()) continue;

			std::stringstream ssin(line);
			ssin >> key >> e;
			assert(e == "=" && "bad format");
			std::getline(ssin, val);
			// strip
			string_strip(val);

			if (!val.empty()) umapData_.insert({ key, val });
		}
	}

	// single value
	std::string getString(
		const std::string& key, const std::string& defval = "") const {
		auto iter = umapData_.find(key);
		return iter == umapData_.end() ? defval : iter->second;
	}
	template <typename T = double>
	T getNumber(const std::string& key, T defval = T(0)) const {
		auto str = getString(key, "");
		return str.empty() ? defval : static_cast<T>(std::stod(str));
	}

	bool getBoolean(const std::string& key, bool defval = false) {
		auto str = getString(key, "");
		if (str.empty()) return defval;
		try {
			if (str == "false" || std::stoi(str) == 0) return false;
			return true;
		} catch (std::exception e) {
			return true;
		}
	}

	// value list
	std::vector<std::string> getStringList(const std::string& key) const {
		auto iter = umapData_.find(key);

		return iter == umapData_.end() ? std::vector<std::string>()
			: string_split(iter->second, ", ");
	}

	template <typename T = double>
	std::vector<T> getNumberList(const std::string& key) const {
		auto vecStrs = getStringList(key);
		std::vector<T> vecNums(vecStrs.size());
		std::transform(vecStrs.begin(), vecStrs.end(), vecNums.begin(),
			[](const std::string& str) { return static_cast<T>(std::stod(str)); });
		return vecNums;
	}

	// may not use
	bool add(const std::string& key, const std::string& val) {
		auto iter = umapData_.find(key);
		if (iter == umapData_.end()) {
			umapData_.insert(std::make_pair(key, val));
			return true;
		}
		return false;
	}
	bool save(
		const std::string& filename, const std::string& headers = "") const {
		return save(
			filename, headers, std::unordered_map<std::string, std::string>());
	}
	bool save(const std::string& filename, const std::string& headers,
		const std::unordered_map<std::string, std::string>& umapValDesc) const {
		std::ofstream fout(filename);
		if (!fout.is_open()) return false;
		// todo: headers may contain new lines
		if (!headers.empty()) fout << headers << "\n";
		for (const auto& pr : umapData_) {
			auto iter = umapValDesc.find(pr.first);
			if (iter != umapValDesc.end()) fout << "# " << iter->second << "\n";
			fout << pr.first << "\t=\t" << pr.second << "\n";
		}

		return true;
	}

	void print(std::ostream& os) const {
		os << umapData_.size() << " values loaded\n";
		for (const auto& pr : umapData_)
			os << pr.first << "\t=\t" << pr.second << "\n";
	}

private:
	std::unordered_map<std::string, std::string> umapData_;
};

// no cache used, for very light use
template <typename T>
inline T getNumberFrom(
	const std::string& filename, const std::string& keyname, T defval = T(0)) {
	ConfigParser cp(filename);
	return cp.getNumber<T>(keyname, defval);
}
inline std::string getStringFrom(const std::string& filename,
	const std::string& keyname, const std::string& defval = "") {
	ConfigParser cp(filename);
	return cp.getString(keyname, defval);
}

// simple binary map
template <typename K, typename V>
class bimap {
public:
	bool insert(const K& k, const V& v) {
		if (left_.count(k) || right_.count(v)) return false;
		left_[k] = v;
		right_[v] = k;

		return true;
	}

	void erase_key(K k) {
		auto iter = left_.find(k);
		if (iter != left_.end()) {
			right_.erase(iter->second);
			left_.erase(iter);
		}
	}

	void erase_value(V v) {
		auto iter = right_.find(v);
		if (iter != right_.end()) {
			left_.erase(iter->second);
			right_.erase(iter);
		}
	}

	std::size_t size() const { return left_.size(); }

	const std::map<K, V>& left() const { return left_; }
	const std::map<V, K>& right() const { return right_; }

private:
	std::map<K, V> left_;
	std::map<V, K> right_;
};

// simple unique priority queue
template <typename T>
class unique_priority_queue {
public:
	bool push(const T& t) {
		if (set_.insert(t).second) {
			queue_.push(t);
			return true;
		}

		return false;
	}

	T pop() {
		assert(!queue_.empty());
		T t = queue_.top();
		queue_.pop();
		set_.erase(t);

		return t;
	}

	std::size_t size() const { return queue_.size(); }

	bool empty() const { return queue_.empty(); }

private:
	std::priority_queue<T> queue_;
	std::set<T> set_;
};

// simple array2d
template<typename T>
class array2d {
public:
	array2d(int rows, int cols) : rows_(rows), cols_(cols) {
		data_ = new T[rows * cols];
	}
	array2d(int rows, int cols, T defval) : array2d(rows, cols) {
		for (int i = 0; i < rows * cols; ++i) data_[i] = defval;
	}
	~array2d() { if (data_) delete[] data_; }

	int rows() const { return rows_; }
	int cols() const { return cols_; }
	const T* data() const { return data_; }
	int size() const { return rows_ * cols_; }

	T* operator[](int r) { return data_ + r * cols_; }
	T& at(int r, int c) { return data[r * cols_ + c]; }
	const T at(int r, int c) const { return data[r * cols_ + c]; }

private:
	T* data_{ nullptr };
	int rows_, cols_;
};
}
