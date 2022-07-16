#pragma once
#include "cppython/object.hpp"

namespace cppython {
   class Import : public Object {
   public:
      Object getAttr(const char* name);
      bool hasAttr(const char* name) const;

      static Import Make(const char* name, const types::List<types::String>& fromList);
   private:
      using Object::Object;
   };
} // namespace cppython