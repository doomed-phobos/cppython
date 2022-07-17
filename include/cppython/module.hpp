#pragma once
#include "cppython/object.hpp"

namespace cppython {
   namespace types {
      class List;
   }

   class Module : public Object {
   public:
      Object getAttr(const char* name);
      bool hasAttr(const char* name) const;

      static Module Make(const char* name, const types::List& fromList);
   private:
      Module(PyObject* pyObj);
   };
} // namespace cppython