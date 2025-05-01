#include <pybind11/pybind11.h>
#include <SofaOffscreenCamera/OffscreenCamera.h>
#include <SofaPython3/Sofa/Core/Binding_Base.h>

namespace py = pybind11;
template <typename T> using py_shared_ptr = sofapython3::py_shared_ptr<T>;

void add_offscreen_camera_to_module(pybind11::module &m) {
    using BaseCamera =  sofa::component::visual::BaseCamera;

    py::class_< OffscreenCamera, BaseCamera, py_shared_ptr<OffscreenCamera> > c(m, "OffscreenCamera");
    c.def(py::init());
    c.def("save_frame", &OffscreenCamera::save_frame, py::arg("filepath"));
}