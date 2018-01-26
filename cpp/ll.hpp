// std headers
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <memory>
#include <chrono>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>

#include <exception>
#include <functional>
#include <algorithm>
#include <thread>
#include <mutex>

#include <cassert>
#include <ctime>

// macros
#define LL_ABORT(...) throw(std::runtime_error(__VA_ARGS__))
#define LL_LOG std::cout<<"["<<__FILE__<<"-"<<__LINE__<<"]: "

namespace ll{
    template <typename T>
    void dump(const std::vector<T>& vec, const std::string& filename=""){
        std::ofstream fout(filename.c_str());
        assert(fout.is_open());
        for(const auto& v: vec) 
            fout<<v<<"\n";
    }

    template<typename T>
    void dump(const T* data, std::size_t cnt, const std::string& filename=""){
        std::ofstream fout(filename.c_str());
        assert(fout.is_open());
        for(size_t i=0; i<cnt; ++i) 
            fout<<data[i]<<"\n";
    }

    void waitKey(const std::string& str=""){
#if 0
        std::string line    =   "read -p '"+(str.empty()?"press any key":str)+"'";
        system(line.c_str());
#else
        std::cout<<(str.empty()? "press any key": str);
        int i;
        std::cin>>i;
#endif
    }

    template<typename T>
    T rand(T maxval){
        return std::rand()/T(RAND_MAX)*maxval;
    }
    template<typename T>
    T randrange(T minval, T maxval){
        return std::rand()/T(RAND_MAX)*(maxval-minval)+minval;
    }
}
