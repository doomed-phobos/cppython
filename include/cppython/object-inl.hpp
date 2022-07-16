namespace cppython {
   template<typename R, typename... Args>
   inline std::enable_if_t<types::is_ctype_v<R>, R> Object::callMethod(const char* name, Args&&... args) {
      Object obj = PyObject_CallMethodObjArgs(
         *this,
         types::String(name).obj(),
         ((PyObject*)args.obj())...,
         NULL
      );

   }
} // namespace cppython