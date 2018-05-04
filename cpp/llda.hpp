#include "ll.hpp"


#define LL_DA_MEAN(beg, end, type) std::accumulate(beg, end, type(0))/(end-beg)
#define LL_DA_VARIANCE(beg, end) ll::da::variance(beg, end)

namespace ll{ namespace da{

template<typename IT, typename T=double>
T mean(IT beg, IT end){
    return std::accumulate(beg, end, T(0))/(end-beg);
}

template<typename IT, typename T=double>
T variance(IT beg, IT end){
    T m =   mean<IT, T>(beg, end);
    return std::accumulate(beg, end, T(0), [&m](const T& t, const T& v){
        return t+(v-m)*(v-m);
    })/(end-beg);
}

}}