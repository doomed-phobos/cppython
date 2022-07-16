#include <stdexcept>
#include <iostream>

namespace cppython {
   namespace priv {
      template<typename... Args>
      static void pyTuple_helper(const Object& obj) {}

      template<typename Arg0, typename... Args>
      static void pyTuple_helper(const Object& obj, Arg0&& value, Args&&... args) {
         pyTuple_helper(obj, std::forward<Args>(args)...);
         PyTuple_SetItem(obj, sizeof...(Args), value.pyObj());
      }
   } // namespace priv

   namespace types {
      template<typename T>
      inline CType<T>::CType(const T& value) :
         m_value(value) {}

      template<>
      inline Object CType<bool>::pyObj() const {
         return PyBool_FromLong(m_value);
      }

      template<>
      inline Object CType<const char*>::pyObj() const {
         return PyUnicode_FromString(m_value);
      }

      template<typename... Args>
      inline Tuple::Tuple(Args&&... args) :
         m_obj(PyTuple_New(sizeof...(Args))) {
         priv::pyTuple_helper(m_obj, std::forward<Args>(args)...);
      }

      template<typename T,
         std::enable_if_t<is_ctype_v<T>, int> = 0>
      inline void Dict::addItem(const char* key, T&& value) {
         PyDict_SetItemString(m_obj, key, value.pyObj());
      }
   } // namespace types

   template<typename... Args>
   inline Object Object::callMethod(const char* name, Args&&... args) {
      return PyObject_CallMethodObjArgs(
         *this,
         types::String(name).pyObj(),
         ((PyObject*)args.pyObj())...,
         NULL);
   }
} // namespace cppython