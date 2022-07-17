#pragma once
namespace cppython {
   class Object;

   namespace types {
      class List;
   }

   Object load_module(const char* name, const types::List& fromList);
} // namespace cppython