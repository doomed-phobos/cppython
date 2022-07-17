#include "cppython/instance.hpp"

#include "cppython/callable.hpp"

#include <Python.h>
#include <cstdarg>

namespace cppython {
   Instance::Instance(PyObject* pyObj) :
      Object(pyObj) {}

   Object Instance::callMethod(const char* name,
         const types::Tuple& args,
         const std::optional<types::Dict>& kwargs) {
      if(!args || args.size() <= 0)
         return Object();

      Object obj = getAttr(name);
      if(!obj.canBe<Callable>())
         return Object();

      PyObject* py_kwargs = nullptr;
      if(kwargs.has_value())
         py_kwargs = kwargs.value();

      return Object::MakeFromRaw(
         PyObject_Call(obj, args, py_kwargs)
      );
   }  
} // namespace cppython