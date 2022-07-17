#pragma once
#include "cppython/object.hpp"
#include "cppython/ctypes.hpp"

#include <optional>

namespace cppython {
   class Instance : public Object {
   public:
      Object callMethod(const char* name,
         const types::Tuple& args,
         const std::optional<types::Dict>& kwargs = std::nullopt);
   private:
      friend class Callable;

      Instance(PyObject*);
   };
} // namespace cppython