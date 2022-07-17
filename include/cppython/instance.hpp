#pragma once
#include "cppython/object.hpp"

#include "cppython/ctypes.hpp"

namespace cppython {
   class Instance : public Object {
   public:
      template<typename... Args>
      Object callMethod(const char* name, Args&&... args) {
         return
            Object::MakeFromRaw(_callMethodPtr(
               *this,
               types::String(name),
               ((PyObject*)args)...,
               NULL));
      }
   private:
      friend class Callable;

      typedef PyObject* (*wrapper_t)(PyObject*, PyObject*, ...);
      static wrapper_t _callMethodPtr;

      Instance(PyObject*);
   };
} // namespace cppython