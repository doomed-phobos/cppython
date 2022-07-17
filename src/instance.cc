#include "cppython/instance.hpp"

#include "cppython/ctypes.hpp"

#include <Python.h>
#include <cstdarg>

namespace cppython {
   Instance::wrapper_t Instance::_callMethodPtr = (wrapper_t)&PyObject_CallMethodObjArgs;
   
   Instance::Instance(PyObject* pyObj) :
      Object(pyObj) {}
} // namespace cppython