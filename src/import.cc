#include "cppython/import.hpp"

namespace cppython {
   Object Import::getAttr(const char* name) {
      return PyObject_GetAttrString(*this, name);
   }
   bool Import::hasAttr(const char* name) const {
      return PyObject_HasAttrString(*this, name);
   }

   Import Import::Make(const char* name, const types::List<types::String>& fromList) {   
      PyObject* mod = PyImport_ImportModuleEx(name, nullptr, nullptr, fromList.obj());
      if(!mod) {
         // TODO: Invoke error callback?
         return nullptr;
      }

      return mod;
   }
} // namespace cppython