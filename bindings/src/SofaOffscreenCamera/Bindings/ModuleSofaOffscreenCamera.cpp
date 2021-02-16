#include <pybind11/pybind11.h>

void add_offscreen_camera_to_module(pybind11::module &m);

PYBIND11_MODULE(SofaOffscreenCamera, m) {
    add_offscreen_camera_to_module(m);
}