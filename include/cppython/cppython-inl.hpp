template<typename T>
inline Object Object::callMethod_helper(T&& arg) {
   using type = std::decay_t<T>;
   return types::CType<type>(std::forward<type>(arg)).toPython();
}

template<typename... Args>
inline Object Object::callMethod(const char* name, Args&&... args) {
   return PyObject_CallMethodObjArgs(
      (PyObject*)*this,
      (PyObject*)types::String(std::move(name)).toPython(),
      (PyObject*)Object::callMethod_helper(std::forward<Args>(args))...,
      NULL);
}

namespace types {
   template<>
   inline Object CType<bool>::toPython() const {
      return PyBool_FromLong((long)m_value);
   }

   template<>
   inline Object CType<const char*>::toPython() const {
      return PyUnicode_FromString(m_value);
   }

   template<typename T>
   inline CType<T>::CType(T&& value) :
      m_value(std::forward<T>(value)) {}

   template<typename T>
   inline CType<std::vector<T>>::CType(const std::initializer_list<T>& initializer_list) :
      m_value(initializer_list.begin(), initializer_list.end()) {}

   template<typename T>
   inline CType<std::vector<T>>::CType(container_t&& value) :
      m_value(std::move(value)) {}

   template<typename T>
   inline Object CType<std::vector<T>>::toPython() const {
      Object obj(PyList_New(0));
      for(auto&& item : m_value)
         PyList_Append((PyObject*)obj, (PyObject*)CType<T>(std::forward<T>((T)item)).toPython());

      return obj;
   }
} // namespace types