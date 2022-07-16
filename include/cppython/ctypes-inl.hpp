#include "cppython/ctype_traits.hpp"
#include <vector>

namespace cppython {
   namespace types {
      template<typename T>
      class CType;

      namespace priv {
         class CType_base {
         public:
            const Object& obj() const {return m_obj;}
         protected:
            CType_base(PyObject* pyObj) :
               m_obj(pyObj) {}
         private:
            Object m_obj;
         };

         template<typename... Args>
         static void pyTuple_helper(const Object& obj) {}

         template<typename Arg0, typename... Args>
         static void pyTuple_helper(const Object& obj, Arg0&& value, Args&&... args) {
            pyTuple_helper(obj, std::forward<Args>(args)...);
            PyTuple_SetItem(obj, sizeof...(Args), value.obj());
         }
      } // namespace priv

      template<typename T>
      class CType {};

      template<>
      class CType<bool> : public priv::CType_base {
      public:
         explicit CType(bool value) :
            priv::CType_base(PyBool_FromLong(value)) {}

         static bool EqualsType(PyObject* pyObj) {
            return PyBool_Check(pyObj);
         }
      };

      template<>
      class CType<const char*> : public priv::CType_base {
      public:
         explicit CType(const char* value) :
            priv::CType_base(PyUnicode_FromString(value)) {}

         static bool EqualsType(PyObject* pyObj) {
            return PyUnicode_Check(pyObj);
         }
      };


      template<typename T,
         std::enable_if_t<is_ctype_v<T>, int> = 0>
      class List : public priv::CType_base {
      public:
         List(const std::initializer_list<T>& initializer_list) :
            List(std::vector<T>(initializer_list.begin(), initializer_list.end())) {}
         List(const std::vector<T>& container) :
            priv::CType_base(PyList_New(0)) {
               for(auto& item : container)
                  PyList_Append(obj(), item.obj());
            }

         static bool EqualsType(PyObject* pyObj) {
            return PyList_Check(pyObj);
         }
      };

      class Tuple : public priv::CType_base {
      public:
         template<typename... Args>
         Tuple(Args&&... args) :
            priv::CType_base(PyTuple_New(sizeof...(Args))) {
            priv::pyTuple_helper(obj(), std::forward<Args>(args)...);
            }
         
         static bool EqualsType(PyObject* pyObj) {
            return PyTuple_Check(pyObj);
         }
      };

      class Dict : public priv::CType_base {
      public:
         Dict() :
            priv::CType_base(PyDict_New()) {}

         template<typename T,
            std::enable_if_t<is_ctype_v<T>, int> = 0>
         void addItem(const char* key, T&& value) {
            PyDict_SetItemString(obj(), key, value.obj());
         }

         static bool EqualsType(PyObject* pyObj) {
            return PyDict_Check(pyObj);
         }
      };

      typedef CType<const char*> String;
      typedef CType<bool> Bool;
   } // namespace types
} // namespace cppython