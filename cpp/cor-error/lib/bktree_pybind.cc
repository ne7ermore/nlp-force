#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "bktree.h"

using namespace BkTree;
namespace py = pybind11;

PYBIND11_MODULE(bktree_pybind, m) {
    py::class_<BkTree::tree>(m, "tree")
        .def(py::init<>())
        .def("add", &BkTree::tree::add)
        .def("levenshtein_distance", &BkTree::tree::levenshtein_distance)
        .def("longest_common_seq", &BkTree::tree::longest_common_seq)
        .def("search", &BkTree::tree::search);
}