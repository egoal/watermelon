#pragma once

// std headers
#include <iostream>
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

#define LL_TOFILE(f, ...) \
    { std::ofstream fout(f, std::ios::out|std::ios::app); \
    ll::CommaOS cos(fout); \
    cos, __VA_ARGS__; }

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

    inline void waitKey(const std::string& str=""){
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
        return T(std::rand()/double(RAND_MAX)*maxval);
    }
    template<typename T>
    T randrange(T minval, T maxval){
        return T(std::rand()/double(RAND_MAX)*(maxval-minval))+minval;
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
                if(line.empty() || line[0]=='#')
                    continue;
                
                std::stringstream ssin(line);
                ssin>>key>>e>>val;
                umapData_.insert({key, val});
            }

        }

        int getInt(const std::string& key, int defval=0) const{
            auto iter   =   umapData_.find(key);
            return iter==umapData_.end()? defval: std::stoi(iter->second);
        }
        double getDouble(const std::string& key, double defval=0.0) const{
            auto iter   =   umapData_.find(key);
            return iter==umapData_.end()? defval: std::stod(iter->second);
        }
        std::string getString(const std::string& key, const std::string& defval="") const{
            auto iter   =   umapData_.find(key);
            return iter==umapData_.end()? defval: iter->second;
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
            save(filename, headers, std::unordered_map<std::string, std::string >());
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
    inline int getIntFrom(const std::string& filename, const std::string& keyname, 
        int defval=0){
        ConfigParser cp(filename);
        return cp.getInt(keyname, defval);
    }
    inline std::string getStringFrom(const std::string& filename, 
        const std::string& keyname, const std::string& defval=""){
        ConfigParser cp(filename);
        return cp.getString(keyname, defval);
    }

    
}
