#include "cppython/ctypes.hpp"

#include <Python.h>

namespace cppython::types {
   namespace priv {
      PyBool::type PyBool::FromPy(PyObject* pyObj) {
         return PyLong_AsLong(pyObj);
      }

      PyObject* PyBool::ToPy(type value) {
         return PyBool_FromLong(value);
      }

      bool PyBool::CheckType(const Object& obj) {
         return PyBool_Check(obj);
      }

      PyString::type PyString::FromPy(PyObject* pyObj) {
         return PyUnicode_AsUTF8(pyObj);
      }

      PyObject* PyString::ToPy(const type& value) {
         return PyUnicode_FromString(value.c_str());
      }

      bool PyString::CheckType(const Object& obj) {
         return PyUnicode_Check(obj);
      }

      PyList::type PyList::FromPy(PyObject* pyObj) {
         type t;
         for(Py_ssize_t i = 0; i < PyList_GET_SIZE(pyObj); ++i)
            t.push_back(Object::MakeFromBorrow(PyList_GET_ITEM(pyObj, i)));
         
         return t;
      }

      PyObject* PyList::ToPy(const type& value) {
         PyObject* pyObj = PyList_New(0);
         
         for(const auto& item : value)
            PyList_Append(pyObj, item);
         
         return pyObj;
      }

      bool PyList::CheckType(const Object& obj) {
         return PyList_Check(obj);
      }

      PyDict::type PyDict::FromPy(PyObject* pyObj) {
         type t;
         PyObject* key, *value;
         Py_ssize_t pos = 0;
         while(PyDict_Next(pyObj, &pos, &key, &value))
            t.emplace(PyString::FromPy(key), Object::MakeFromBorrow(value));

         return t;
      }

      PyObject* PyDict::ToPy(const type& t) {
         PyObject* obj = PyDict_New();
         for(const auto& [key, value] : t)
            PyDict_SetItemString(obj, key.c_str(), value);

         return obj;
      }

      bool PyDict::CheckType(const Object& obj) {
         return PyDict_Check(obj);
      }

      PyTuple::type PyTuple::FromPy(PyObject* pyObj) {
         type t;
         for(Py_ssize_t i = 0; i < PyTuple_GET_SIZE(pyObj); ++i)
            t.push_back(Object::MakeFromBorrow(PyTuple_GET_ITEM(pyObj, i)));
         
         return t;
      }

      PyObject* PyTuple::ToPy(const type& t) {
         PyObject* obj = PyTuple_New(t.size());
         for(size_t i = 0; i < t.size(); ++i)
            PyTuple_SetItem(obj, i, t[i]);

         return obj;
      }

      bool PyTuple::CheckType(const Object& obj) {
         return PyTuple_Check(obj);
      }
   } // namespace priv

   List::List(const std::initializer_list<Object>& il) :
      List(type(il.begin(), il.end())) {}

   void Dict::addItem(const std::string& key, Object&& obj) {
      PyDict_SetItemString(*this, key.c_str(), obj);
   }

   Tuple::Tuple(const std::initializer_list<Object>& il) :
      Tuple(type(il.begin(), il.end())) {}
} // namespace cppython::types