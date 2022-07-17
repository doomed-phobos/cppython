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

         type value() const {return isValid() ? T::FromPy(*this) : type();}

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

   CType_BEGIN(Tuple)
      public:
         Tuple(const std::initializer_list<Object>& il);
         
         Object getItem(int index) const;
         int size() const;
   CType_END

   CType_BEGIN(Dict)
      public:
         void addItem(const char* key, Object&& obj);
         Object getItem(const char* key) const;
   CType_END
} // namespace cppython::types