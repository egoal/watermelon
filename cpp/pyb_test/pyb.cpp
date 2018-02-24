#include <iostream>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py    =   pybind11;

// export module
int add(int i, int j){ return i+j; }

PYBIND11_MODULE(pyb, m){
    m.doc() =   "test pybind";

    m.def("add", &add, "add two number");
    m.attr("version")   =   "1.1";

    m.def("key_add", &add, "keyword add", py::arg("a"), py::arg("b")=2);
}

// call python3 interperter
int main(){
    py::scoped_interpreter py3;

    // module
    py::module sys  =   py::module::import("sys");
    py::print(sys.attr("path"));

    py::module pyb  =   py::module::import("pyb");
    
    // cast
    // py::object po   =   py::cast(MyClass());
    std::cout<<pyb.attr("add")(1, 2).cast<int>()<<"\n";

    return 0;
}
