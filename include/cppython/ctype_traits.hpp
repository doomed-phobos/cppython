#pragma once
#include <type_traits>

namespace cppython {
   namespace types {
      namespace priv {
         struct CType_base;
      } // namespace priv

      template<typename T>
      inline constexpr bool is_ctype_v =
         std::is_base_of_v<priv::CType_base, T> && std::is_function_v<decltype(T::EqualsType)>;
   } // namespace types
} // namespace cppython