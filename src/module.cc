#include "cppython/module.hpp"

#include "cppython/ctypes.hpp"

#include <Python.h>

namespace cppython {
   Module::Module(PyObject* pyObj) :
      Object(Object::MakeFromRaw(pyObj)) {}

   Object Module::getAttr(const char* name) {
      return Object::MakeFromRaw(PyObject_GetAttrString(*this, name));
   }
   bool Module::hasAttr(const char* name) const {
      return PyObject_HasAttrString(*this, name);
   }

   Module Module::Make(const char* name, const types::List& fromList) {   
      PyObject* mod = PyImport_ImportModuleEx(name, nullptr, nullptr, fromList);
      if(!mod)
         return nullptr;

      return mod;
   }
} // namespace cppython