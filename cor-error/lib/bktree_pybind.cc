#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "bktree.h"

using namespace Bkt;
namespace py = pybind11;

PYBIND11_MODULE(bktree_pybind, m) {
    py::class_<Bkt::Tree>(m, "tree")
        .def(py::init<>)
        .def("read_file", &Bkt::Tree::read_file)
        .def("add", &Bkt::Tree::add)
        .def("get", &Bkt::Tree::get);   
}