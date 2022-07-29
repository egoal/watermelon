```cpp

// std::reference_wrapper 可以保存容器引用

std::list<int> l;
std::vector<std::reference_wrapper<int> > v(l.begin(), l.end());
std::random_shuffle(v.begin(), v.end()); // std::random_shuffle本身只能作用于随机访问器，std::list不能直接使用
// 这样等于创建了一个view，原l是不变的
for(auto& i: l) i*=2;
for(auto& ri: v) ri.get() *=2;
// 如果直接修改了值，两者是同步的（那必然的，毕竟是引用啊）

std::optional<std::reference_wrapper<int>> // 这样就能让std::optional适应引用


// 几个不常用重载
T& operator++(); // ++t
friend T& operator++(T&);
T& operator++(int); // t++
friend T& operator++(T&, int);

T operator-() const; // -t

// 不怎么用的多态，实际上也应该避免干重复名的事
class A{
public:
    A(int a);
    
    virtual void vf();
    void f();
    void f(int);
   
    int a;
}

class B: public A{
public:
    using A::A; // 直接使用A的全部构造函数：new B(1)
    
    void vf() override;
    void f(); // 只会覆盖并隐藏A::f(int)
}

A* pa = new B(1);
pa->vf(); // B::vf
pa->f(); // A::f
pa->a; // A::a

B* pb = dynamic_cast<B*>(pa);
pb->a; // b::a
pb->f(1); // 并没有B::f(int)
pb->A::f(A);
pb->A::f();
p->f(); // B::f()
```



`#include <ctime> <chrono>`

```cpp
// 本地时间
auto t = time(nullptr);
auto timeinfo = localtime(&t);
timeinfo->tm_hour; // tm_minuter, ...
std::cout<< asctime(timeinfo);

// 运行时间, 见 ll::TimeCounter;
```



`#include <algorithm> <numeric>`

```cpp
// 参见 ll::mapf, ll::filter, ll::fold, ll::reduce, ll::max_by, ll::zip, ...

std::adjacent_find(begin, end, [](int a, int b){ return a+b == 100; }); // 返回成立时前者的迭代器

std::includes // 两段【已排序】区间之间的包含关系
std::merge // 合并【已排序】区间
std::mismatch(f1, l1, f2, bop){
    while(f1!= l1 && bop(*f1, *f2)) {
        ++f1, ++f2;
    }
    return std::make_pair(f1, f2);
}

std::inner_product(f1, l1, f2, v, bop1, bop2){
    while(f1!= l1){
        v = bop1(v, bop2(*f1, *f2)); // 注意到，op2=*, op1=+时即：累加对应项相乘的结果，就好像向量内积一样
        ++f1, ++f2;
    }
    return v;
}
```



`#include <random>`

```cpp
std::random_device rd{};
std::mt19937 gen; // {rd();

std::normal_distribution<double> d{0, 1};
LL_REPEAT(10){ std::cout<< d(gen)<< ", "; }
```

