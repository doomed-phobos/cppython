#include "cppython/callable.hpp"

#include "cppython/ctypes.hpp"
#include "cppython/instance.hpp"

#include <Python.h>

namespace cppython {
   bool Callable::CheckType(const Object& obj) {
      return PyCallable_Check(obj);
   }

   Instance Callable::callObject() {
      return PyObject_CallObject(*this, nullptr);
   }

   Instance Callable::callObject(const types::Tuple& t) {
      return PyObject_CallObject(*this, t);
   }
} // namespace cppython