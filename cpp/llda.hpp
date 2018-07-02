#include "ll.hpp"

namespace ll{ namespace da{

enum class cmp{
    less, equal, greater
};

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

//! this will change the range!
template<typename IT, typename T=double>
std::vector<T> quantile(IT beg, IT end, const std::vector<double>& vecRatios){
    auto cnt    =   static_cast<std::size_t >(std::distance(beg, end));

    double maxRt    =   *(std::max_element(vecRatios.begin(), vecRatios.end()));
    assert(maxRt<=1. && "bag range");
    auto mIdx   =   static_cast<std::size_t >(cnt*maxRt);
    std::partial_sort(beg, beg+mIdx+1, end);
    
    std::vector<T> vec(vecRatios.size());
    std::transform(vecRatios.begin(), vecRatios.end(), vec.begin(), 
        [&beg, cnt](double rt){ return *(beg+int(cnt*rt)); });

    return vec;
}

template<typename BOP> 
std::vector<std::tuple<int, int, double > > match_bf(int maxidx0, int maxidx1, BOP bop, bool crossCheck=false){
    assert(maxidx0>0 && maxidx1>0);
    
    std::vector<std::tuple<int, int, double > > matches;

    auto rng0   =   ll::range(maxidx0);
    auto rng1   =   ll::range(maxidx1);
    for(auto& i: rng0){
        int minidx  =   -1;
        double mindis   =   std::numeric_limits<double>::max();
        for(auto& j: rng1){
            double dis  =   bop(i, j);
            if(dis<mindis){
                mindis  =   dis;
                minidx  =   j;
            }
        }

        if(crossCheck && std::any_of(rng0.begin(), rng0.end(), 
            [&](int i){ return bop(i, minidx)<mindis; }))
            continue;

        matches.push_back(std::make_tuple(i, minidx, mindis));
    }

    return matches;
}

}}