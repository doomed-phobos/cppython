#pragma once
#include <functional>

namespace cppython::priv {
   typedef std::function<void(const std::string& /*msg*/)> error_callback_t;
   void show_error();
   void set_error_callback(error_callback_t&& f);
} // namespace cppython::priv

#define CPPYTHON_SHOW_ERROR() cppython::priv::show_error()
#define CPPYTHON_SET_ERROR(callback) cppython::priv::set_error_callback(callback)