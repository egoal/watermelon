#pragma once

// std headers
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <memory>
#include <chrono>
#include <numeric>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <queue>
#include <tuple>

#include <exception>
#include <functional>
#include <algorithm>
#include <thread>
#include <mutex>
#include <utility>

#include <cassert>
#include <ctime>

// macros
#define LL_BAN_COPY(X) \
    X(const X& )    =   delete; \
    X& operator=(const X& ) =   delete

#define LL_ABORT(...) throw(std::runtime_error(__VA_ARGS__))
#define LL_ASSERT(...) if(!(__VA_ARGS__)) LL_ABORT(#__VA_ARGS__)

#define __FILENAME__ ll::__current_filename(__FILE__)
#define LL_LOG std::cerr<<"["<<__FILENAME__<<": "<<__LINE__<<"] "

#define LL_NOTNULL(x) {if(!x) LL_ABORT("null check failed: "+#x);} x

#define LL_ABS(x) ((x)>0?(x):(-x))

#define LL_TOFILE(f, ...) \
    { std::ofstream fout(f, std::ios::out|std::ios::app); \
    ll::CommaOS cos(fout); \
    cos, __VA_ARGS__; }

#define LL_REPEAT(n) for(int __i__=0; __i__<n; ++__i__)

namespace ll{

    inline std::string __current_filename(const std::string& file){
        auto pos    =   file.rfind('/');
        return file.substr(pos==std::string::npos? 0: pos+1);
    }

#if __cplusplus<201402L
    //* before c++14
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args){
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
#endif

    // range vector
    template<typename T>
    std::vector<T> range(T e){
        std::vector<T> vec(e);
        std::iota(vec.begin(), vec.end(), 0);
        return vec;
    }
    template<typename T>
    std::vector<T> range(T s, T e, T d){
        assert((e-s)/d >0 && "invalid range call");
        std::vector<T> vec(int((e-s)/d), d);
        vec[0]  =   s;
        std::partial_sum(vec.begin(), vec.end(), vec.begin());
        return vec;
    }

    template<typename T>
    T clamp(T val, T low, T high){
        assert(low<=high && "bad arguments");
        if(val<low) return low;
        if(val>high) return high;
        return val;
    }

    template<typename T>
    T rand(T maxval){
        return T(std::rand()/double(RAND_MAX)*maxval);
    }
    template<typename T>
    T randrange(T minval, T maxval){
        return T(std::rand()/double(RAND_MAX)*(maxval-minval))+minval;
    }

    //* tuple print
    template<typename Tuple, std::size_t N>
    struct __tuple_printer{
        static void print(const Tuple& t){
            __tuple_printer<Tuple, N-1>::print(t);
            std::cout<<", "<<std::get<N-1>(t);
        }
    };
    template<typename Tuple>
    struct __tuple_printer<Tuple, 1>{
        static void print(const Tuple& t){
            std::cout<<std::get<0>(t);
        }
    };
    template<typename... Args> void print(const std::tuple<Args...>& t){
        std::cout<<"(";
        __tuple_printer<decltype(t), sizeof...(Args)>::print(t);
        std::cout<<")";
    }

    //* simple unique patch
    template<typename IT, typename BOP> IT unique(IT beg, IT end, BOP eql){
        if(beg==end) return end;

        IT cur  =   beg;
        IT result   =   beg+1;
        while(++cur!=end){
            if(std::find_if(beg, result, [&](decltype(*cur) value){
                    return eql(value, *cur);
                })==result){
                *result =   *cur;
                ++result;
            }
        }

        return result;
    }
    template<typename IT> IT unique(IT beg, IT end){
        if(beg==end) return end;

        IT cur  =   beg;
        IT result   =   beg+1;
        while(++cur!=end){
            if(std::find(beg, result, *cur)==result){
                *result =   *cur;
                ++result;
            }
        }

        return result;
    }

    //* string function, dont want to relate to boost
    template<typename UOP>
    std::vector<std::string > string_split(const std::string& str, UOP uop){
        std::vector<std::string > vecStrs;
        int s(0), i(0);
        while(i<static_cast<int>(str.size())){
            if(uop(str[i])){
                if(s<i) vecStrs.push_back(str.substr(s, i-s));
                s   =   i+1;
            }
            ++i;
        }
        if(s<i) vecStrs.push_back(str.substr(s, i));

        return vecStrs;
    }
    template<> inline 
    std::vector<std::string > string_split<char>(const std::string& str, char c){
        return string_split(str, [&](char tc){ return tc==c; });
    }
    template<> inline 
    std::vector<std::string > string_split<std::string>(const std::string& str, std::string s){
        return string_split(str, [&](char c){ return s.find(c)!=std::string::npos; });
    }
    template<> inline 
    std::vector<std::string > string_split<const char*>(const std::string& str, const char* ps){
        return string_split(str, std::string(ps));
    }

    //* simple warpper on algo, for fp
    template<typename Container, typename unop> 
    void for_each(Container& c, unop op){
        std::for_each(std::begin(c), std::end(c), op);
    }
    template<typename Container, typename unop> 
    Container& map(Container& c, unop op){
        std::transform(std::begin(c), std::end(c), std::begin(c), op);
        return c;
    }
    template<typename Container, typename binop>
    Container& zip(Container& fc, Container& sc, binop op){
        std::transform(std::begin(sc), std::end(sc), std::begin(sc), std::begin(fc), op);
        return fc;
    }
    template<typename Container, typename con>
    bool exists(Container& c, con co){
        return std::any_of(std::begin(c), std::end(c), co);
    }
    template<typename Container, typename Predicate>
    Container& filter_not(Container& c, Predicate pre){
        c.erase(std::remove_if(std::begin(c), std::end(c), pre), std::end(c));
        return c;
    }
    template<typename Container, typename Predicate>
    Container& filter(Container& c, Predicate pre){
        return filter_not(c, [pre](typename Container::value_type i){
            return !pre(i);
        });
    }

    /*file output*/
    // comma to stream, can be help to macro define
    class CommaOS{
    public:
        CommaOS(std::ostream& os=std::cout, const std::string& deli=""): 
            os_(os), delimiter_(deli){}
        
        template<typename T>
        CommaOS& operator, (T t){
            os_<<t<<delimiter_;
            return *this;
        }

    private:
        std::ostream& os_;
        std::string delimiter_;
    };

    /* file config*/
    // parser for my style
    class ConfigParser{
    public:
        ConfigParser(const std::string& filename){
            reload(filename);
        }
        LL_BAN_COPY(ConfigParser);

        void reload(const std::string& filename){
            std::ifstream fin(filename.c_str());
            assert(fin.is_open());
            std::string line, key, e, val;
            while(std::getline(fin, line)){
                line    =   line.substr(0, line.find(';'));
                if(line.empty()) continue;
                
                std::stringstream ssin(line);
                ssin>>key>>e>>val;
                assert(e=="=" && "bad format");

                if(!val.empty()) umapData_.insert({key, val});
            }

        }

        // single value
        std::string getString(const std::string& key, const std::string& defval="") const{
            auto iter   =   umapData_.find(key);
            return iter==umapData_.end()? defval: iter->second;
        }
        template<typename T=double>
        T getNumber(const std::string& key, T defval=T(0)) const{
            auto str    =   getString(key, "");
            return str.empty()? defval: static_cast<T>(std::stod(str));
        }

        // value list
        std::vector<std::string > getStringList(const std::string& key) const{
            auto iter   =   umapData_.find(key);

            return iter==umapData_.end()? std::vector<std::string >():
                string_split(iter->second, ", ");
        }

        template<typename T=double>
        std::vector<T> getNumberList(const std::string& key) const{
            auto vecStrs    =   getStringList(key);
            std::vector<T> vecNums(vecStrs.size());
            std::transform(vecStrs.begin(), vecStrs.end(), vecNums.begin(), 
                [](const std::string& str){
                    return static_cast<T>(std::stod(str));
                });
            return vecNums;
        }

        // may not use
        bool add(const std::string& key, const std::string& val){
            auto iter   =   umapData_.find(key);
            if(iter==umapData_.end()){
                umapData_.insert(std::make_pair(key, val));
                return true;
            }
            return false;
        }
        bool save(const std::string& filename, const std::string& headers="") const{
            return save(filename, headers, std::unordered_map<std::string, std::string >());
        }
        bool save(const std::string& filename, const std::string& headers, 
            const std::unordered_map<std::string, std::string>& umapValDesc) const{
            std::ofstream fout(filename);
            if(!fout.is_open()) return false;
            //todo: headers may contain new lines
            if(!headers.empty())
                fout<<headers<<"\n";
            for(const auto& pr: umapData_){
                auto iter   =   umapValDesc.find(pr.first);
                if(iter!=umapValDesc.end())
                    fout<<"# "<<iter->second<<"\n";
                fout<<pr.first<<"\t=\t"<<pr.second<<"\n";
            }

            return true;
        }

        void print(std::ostream& os) const{
            os<<umapData_.size()<<" values loaded\n";
            for(const auto& pr: umapData_)
                os<<pr.first<<"\t=\t"<<pr.second<<"\n";
        }
    private:
        std::unordered_map<std::string, std::string > umapData_;

    };

    // no cache used, for very light use
    template<typename T>
    inline T getNumberFrom(const std::string& filename, const std::string& keyname, 
        T defval=T(0)){
        ConfigParser cp(filename);
        return cp.getNumber<T>(keyname, defval);
    }
    inline std::string getStringFrom(const std::string& filename, 
        const std::string& keyname, const std::string& defval=""){
        ConfigParser cp(filename);
        return cp.getString(keyname, defval);
    }
    
}
