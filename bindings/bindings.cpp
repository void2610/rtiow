#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

#include <rtcore/camera.hpp>
#include <rtcore/hittable.hpp>
#include <rtcore/hittable_list.hpp>
#include <rtcore/material.hpp>
#include <rtcore/sphere.hpp>
#include <rtimage/writer.hpp>
#include <rtmath/ray.hpp>
#include <rtmath/vec.hpp>
#include <string>

namespace nb = nanobind;
using namespace rtmath;
using namespace rtcore;

NB_MODULE(_rtiow, m) {
  // --- rtmath ---
  nb::class_<vec3>(m, "Vec3")
      .def(nb::init<>())
      .def(nb::init<double, double, double>())
      .def_prop_ro("x", &vec3::x)
      .def_prop_ro("y", &vec3::y)
      .def_prop_ro("z", &vec3::z)
      .def("length", &vec3::length)
      .def("length_squared", &vec3::length_squared)
      .def(nb::self + nb::self)
      .def(nb::self - nb::self)
      .def(nb::self * nb::self)
      .def(double() * nb::self)
      .def(nb::self * double())
      .def(nb::self / double())
      .def("__repr__", [](const vec3 &v) {
        return "Vec3(" + std::to_string(v.x()) + ", " + std::to_string(v.y()) +
               ", " + std::to_string(v.z()) + ")";
      });
  // Color / Point3は同じ型のエイリアス
  m.attr("Color") = m.attr("Vec3");
  m.attr("Point3") = m.attr("Vec3");
}
