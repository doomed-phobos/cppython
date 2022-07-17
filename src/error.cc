#include "cppython/error.hpp"

#include "cppython/ctypes.hpp"

#include <iostream>
#include <Python.h>
#include <sstream>

namespace cppython::priv {
   static void default_error_callback(const std::string& msg) {
      std::cerr << msg;
   }

   static error_callback_t s_error_callback = default_error_callback;
   
   void set_error_callback(error_callback_t&& f) {
      if(f)
         s_error_callback = std::move(f);
   }

   void show_error() {
      if(!Py_IsInitialized()) return;

      if(PyErr_Occurred()) {
         PyObject* ptype, *pvalue, *ptraceback;
         PyErr_Fetch(&ptype, &pvalue, &ptraceback);
         std::stringstream ss;
         ss << ((PyTypeObject*)ptype)->tp_name << ": " << Object::MakeFromBorrow(pvalue).str() << std::endl;
         if(ptraceback)
            ss << Object::MakeFromBorrow(ptraceback).str() << std::endl;
         s_error_callback(ss.str());

         PyErr_Restore(ptype, pvalue, ptraceback);
      }
   }
} // namespace cppython::priv