// simple test

#include "ll.hpp"
#include "llda.hpp"

void testRange(){
    for(auto& v: ll::range(1., 0., -.2))
        std::cout<<v<<" ";
    std::cout<<"\n";
}

void testMean(){
    int a[]{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int > v(a, a+8);
    std::cout<<ll::da::mean<decltype(v.begin()), int>(v.begin(), v.end())<<"\n";

    std::cout<<ll::da::variance(v.begin(), v.end())<<"\n";
}

void testUnique(){
    int a[]{5, 3, 6, 1, 2, 1, 2, 3, 4, 4, 3, 2, 0, 1};
    std::vector<int > v(a, a+sizeof(a)/sizeof(int));
    // std::function<bool (int, int)> f   =   [](int i1, int i2){ return i1==i2; };
    auto it  =  ll::unique(v.begin(), v.end());
    std::for_each(v.begin(), it, [](int i){ std::cout<<i<<", "; });
}

int main(int, char**){
    testUnique();

    return 0;
}
