```cpp
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

using namespace boost::algorithm;

std::string s= "Boost";
to_upper_copy; to_lower_copy;
erase_first_copy(s, "o"); // Bost
erase_head_copy(s, 2); // ost

std::vector<std::string> vs{ "Boost", "C++", "Libraries" };
join(v, " ");

trim_copy;
trim_copy_if("123Boost345", is_digit());

starts_with; split;

boost::format{"%1%.%2%.%3%"} %12 %5 %2014 // 12.5.2014
```

