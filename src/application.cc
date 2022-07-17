#include "cppython/application.hpp"

#include <Python.h>
#include <stdexcept>

namespace cppython {
   Application::Application() {
      Py_Initialize();
      if(!Py_IsInitialized())
         throw std::runtime_error("Something is wrong at init Python");
   }

   Application::~Application() {
      if(Py_FinalizeEx() < 0)
         perror("WARNING: Py_Finalize isn't zero");
   }
} // namespace cppython