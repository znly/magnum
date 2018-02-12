/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "Magnum/Magnum.h"
#include "Magnum/Math/Functions.h"
#include "Magnum/Math/Matrix3.h"
#include "Magnum/Math/Matrix4.h"

using namespace Magnum;

namespace py = pybind11;

template<class T> std::string repr(const T& value) {
    std::ostringstream out;
    Debug(&out) << value;
    return out.str();
}

PYBIND11_MODULE(magnum, m) {
    m.doc() = "Magnum";

    {
        auto math = m.def_submodule("math");
        math.def("log", static_cast<UnsignedInt(*)(UnsignedInt, UnsignedInt)>(&Math::log), "Integral logarithm");
        math.def("log2", &Math::log2, "Base-2 integral logarithm");
        math.def("dot", [](const Vector3& a, const Vector3& b) {
            return Math::dot(a, b);
        });
        math.def("angle", [](const Vector3& a, const Vector3& b) {
            return Math::angle(a, b);
        });

        py::class_<Vector3>(m, "Vector3")
            .def_static("x_axis", &Vector3::xAxis, "Vector in direction of X axis (right)", py::arg("length") = 1.0f)
            .def_static("y_axis", &Vector3::yAxis, "Vector in direction of Y axis (up)", py::arg("length") = 1.0f)
            .def_static("z_axis", &Vector3::zAxis, "Vector in direction of Z axis (backward)", py::arg("length") = 1.0f)
            .def(py::init<Float, Float, Float>())
            .def(py::init<Float>())
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def("is_zero", &Vector3::isZero)
            .def("is_normalized", &Vector3::isNormalized)
            .def(-py::self)
            .def(py::self += py::self)
            .def(py::self + py::self)
            .def(py::self -= py::self)
            .def(py::self - py::self)
            .def(py::self *= Float{})
            .def(py::self * Float{})
            .def(py::self /= Float {})
            .def(py::self / Float{})
            .def(py::self *= py::self)
            .def(py::self * py::self)
            .def(py::self /= py::self)
            .def(py::self / py::self)
            .def("dot", static_cast<Float(Vector3::*)() const>(&Vector3::dot), "Dot product of the vector")
            .def("length", &Vector3::length)
            .def("length_inverted", &Vector3::lengthInverted)
            .def("normalized", &Vector3::normalized)
            .def("resized", &Vector3::resized)
            .def("projected", &Vector3::projected)
            .def("projected_onto_normalized", &Vector3::projectedOntoNormalized)
            .def("flipped", &Vector3::flipped)
            .def("sum", &Vector3::sum)
            .def("product", &Vector3::product)
            .def("min", &Vector3::min)
            .def("max", &Vector3::max)
            .def("minmax", &Vector3::minmax)
            .def("__repr__", static_cast<std::string(*)(const Vector3&)>(&repr));

        py::class_<Vector4>(m, "Vector4")
            .def(py::init<Float, Float, Float, Float>())
            .def(py::init<Float>())
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def("is_zero", &Vector4::isZero)
            .def("is_normalized", &Vector4::isNormalized)
            .def(-py::self)
            .def(py::self += py::self)
            .def(py::self + py::self)
            .def(py::self -= py::self)
            .def(py::self - py::self)
            .def(py::self *= Float{})
            .def(py::self * Float{})
            .def(py::self /= Float {})
            .def(py::self / Float{})
            .def(py::self *= py::self)
            .def(py::self * py::self)
            .def(py::self /= py::self)
            .def(py::self / py::self)
            .def("dot", static_cast<Float(Vector4::*)() const>(&Vector4::dot), "Dot product of the vector")
            .def("length", &Vector4::length)
            .def("length_inverted", &Vector4::lengthInverted)
            .def("normalized", &Vector4::normalized)
            .def("resized", &Vector4::resized)
            .def("projected", &Vector4::projected)
            .def("projected_onto_normalized", &Vector4::projectedOntoNormalized)
            .def("flipped", &Vector4::flipped)
            .def("sum", &Vector4::sum)
            .def("product", &Vector4::product)
            .def("min", &Vector4::min)
            .def("max", &Vector4::max)
            .def("minmax", &Vector4::minmax)
            .def("__repr__", static_cast<std::string(*)(const Vector4&)>(&repr));

        py::class_<Rad>(m, "Rad")
            .def(py::init<Float>())
            .def("__repr__", static_cast<std::string(*)(const Rad&)>(&repr));

        py::class_<Matrix4>(m, "Matrix4")
            .def_static("from_diagonal", &Matrix4::fromDiagonal)
            .def_static("translation", static_cast<Matrix4(*)(const Vector3&)>(&Matrix4::translation))
            .def_static("scaling", &Matrix4::scaling)
            .def_static("rotation", static_cast<Matrix4(*)(Rad, const Vector3&)>(&Matrix4::rotation))
            .def_static("rotation_x", &Matrix4::rotationX)
            .def_static("rotation_y", &Matrix4::rotationY)
            .def_static("rotation_z", &Matrix4::rotationZ)
            .def_static("reflection", &Matrix4::reflection)
            .def_static("shearing_xy", &Matrix4::shearingXY)
            .def_static("shearing_xz", &Matrix4::shearingXZ)
            .def_static("shearing_yz", &Matrix4::shearingYZ)
            .def(py::init<Vector4, Vector4, Vector4, Vector4>())
            .def(py::init<Float>())
            .def(py::self * py::self)
            .def("transform_point", &Matrix4::transformPoint)
            .def("transform_vector", &Matrix4::transformVector)
            .def("__repr__", static_cast<std::string(*)(const Matrix4&)>(&repr));
    }
}
