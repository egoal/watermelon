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

int main(int, char**){
    testMean();

    return 0;
}
