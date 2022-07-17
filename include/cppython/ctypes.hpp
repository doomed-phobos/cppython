#pragma once
#include "cppython/object.hpp"

#include <unordered_map>
#include <vector>
#include <string>

namespace cppython::types {
   namespace priv {
      struct PyBool {
         using type = bool;
         
         static type FromPy(PyObject*);
         static PyObject* ToPy(type);
         static bool CheckType(const Object&);
      };

      struct PyString {
         using type = std::string;
         
         static type FromPy(PyObject*);
         static PyObject* ToPy(const type&);
         static bool CheckType(const Object&);
      };

      struct PyList {
         using type = std::vector<Object>;

         static type FromPy(PyObject*);
         static PyObject* ToPy(const type&);
         static bool CheckType(const Object&);
      };

      struct PyDict {
         using type = std::unordered_map<std::string, Object>;

         static type FromPy(PyObject*);
         static PyObject* ToPy(const type&);
         static bool CheckType(const Object&);
      };

      struct PyTuple {
         using type = PyList::type;

         static type FromPy(PyObject*);
         static PyObject* ToPy(const type&);
         static bool CheckType(const Object&);
      };

      template<typename T>
      class CType : public Object {
      public:
         using Object::Object;
         using type = typename T::type;
         
         CType(const type& value) :
            Object(T::ToPy(value)) {}

         type value() const {return T::FromPy(*this);}

         static bool CheckType(const Object& obj) {return T::CheckType(obj);}
      };
   } // namespace priv

   #define CType_BEGIN(name) \
      class name : public priv::CType<priv::Py##name> {\
      public: \
         using priv::CType<priv::Py##name>::CType;
   #define CType_END };
   #define CType_SIMPLE_DEFINE(name) \
      CType_BEGIN(name) \
      CType_END

   CType_SIMPLE_DEFINE(Bool)
   CType_SIMPLE_DEFINE(String)
   CType_BEGIN(List)
      public:
         List(const std::initializer_list<Object>& il);
   CType_END

   CType_BEGIN(Dict)
      public:
         Dict();
         void addItem(const std::string& key, Object&& obj);
   CType_END

   CType_BEGIN(Tuple)
      public:
         Tuple(const std::initializer_list<Object>& il);
   CType_END
   
   /*namespace priv {
      class Type_base {
      public:
         Type_base(PyObject* pyObj);
         
         const Object& obj() const;
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
   } // namespace priv*/
} // namespace cppython::types