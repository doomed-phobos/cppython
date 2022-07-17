#include "cppython/module.hpp"

#include "cppython/ctypes.hpp"

#include <Python.h>

namespace cppython {
   Object load_module(const char* name, const types::List& fromList) {   
      PyObject* mod = PyImport_ImportModuleEx(name, nullptr, nullptr, fromList);
      if(!mod)
         return Object();

      return Object::MakeFromRaw(mod);
   }
} // namespace cppython