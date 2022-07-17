#pragma once
#include "cppython/object.hpp"

namespace cppython {
   namespace types {
      class Tuple;
   }

   class Instance;

   class Callable : public Object {
   public:
      Instance callObject();
      Instance callObject(const types::Tuple& t);

      static bool CheckType(const Object& obj);
   };
} // namespace cppython