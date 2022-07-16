#pragma once
#include "cppython/ctype_traits.hpp"

#include <Python.h>

namespace cppython {
   namespace types {
      class Tuple;
   }

   class Object {
   public:
      Object(PyObject* pyObj);
      Object(const Object& o);
      Object(Object&& o);
      virtual ~Object();

      void swap(Object& o);

      bool isValid() const;
      unsigned refCnt() const;
      Object callObject();
      Object callObject(const types::Tuple& args);

      template<typename R,typename... Args>
      std::enable_if_t<types::is_ctype_v<R>, R> callMethod(const char* name, Args&&... args);

      Object& operator=(const Object& o);
      Object& operator=(Object&& o);
      operator PyObject*() const {return m_pyObj;}
      operator bool() const {return isValid();}
   private:
      PyObject* m_pyObj;
   };
} // namespace cppython

#include "cppython/ctypes-inl.hpp"
#include "cppython/object-inl.hpp"