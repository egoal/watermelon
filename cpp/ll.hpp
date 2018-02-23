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
#define LL_BAN_COPY(X) \
    X(const X& )    =   delete; \
    X& operator=(const X& ) =   delete

#define LL_ABORT(...) throw(std::runtime_error(__VA_ARGS__))
#define LL_LOG std::cout<<"["<<__FILE__<<"-"<<__LINE__<<"]: "
#define LL_NOTNULL(x) {if(!x) LL_ABORT("null check failed: "+#x);} x

#define LL_ABS(x) ((x)>0?(x):(-x))

namespace ll{

    template<typename IT>
    std::ostream& dump(IT beg, IT end, std::ostream& os=std::cout, 
        const std::string& deli=", "){
        for(auto it=beg; it!=end; ++it)
            os<<(*it)<<deli;
        return os;
    }

    template<typename IT>
    std::ostream& dump(IT beg, IT end, const std::string& filename, 
        const std::string& deli=", "){
        std::ofstream fout(filename.c_str());
        assert(fout.is_open() && "cannot open file");

        return dump(beg, end, fout, deli);
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
    T clamp(T val, T low, T high){
        if(val<low) return low;
        if(val>high) return high;
        return val;
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
