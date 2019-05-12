#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "bkree.h"

using namespace bkree;
namespace py = pybind11;

PYBIND11_MODULE(bktree_pybind, m) {
    py::class_<Bkt::Tree>(m, "tree")
        .def(py::init<>)
        .def("read_file", &Bkt::Tree::Bkt::read_file)
        .def("add", &Bkt::Tree::Bkt::add)
        .def("get", &Bkt::Tree::Bkt::get);   
}