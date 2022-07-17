#include "cppython/object.hpp"

#include <Python.h>
#include <algorithm>

namespace cppython {
   Object::Object() :
      m_pyObj(nullptr),
      m_unref(false) {}
   Object::Object(PyObject* pyObj, bool unref) :
      m_pyObj(pyObj),
      m_unref(unref) {}
   Object::Object(const Object& o) {
      m_pyObj = o.m_pyObj;
      m_unref = o.m_unref;
      Py_XINCREF(m_pyObj);
   }
   Object::Object(Object&& o) :
      m_pyObj(std::exchange(o.m_pyObj, nullptr)),
      m_unref(std::exchange(o.m_unref, false)) {
   }
   Object::~Object() {
      if(m_unref)
         Py_XDECREF(m_pyObj);
      m_pyObj = nullptr;
      m_unref = false;
   }

   void Object::swap(Object& o) {
      std::swap(m_pyObj, o.m_pyObj);
   }

   bool Object::isValid() const {return m_pyObj;}
   unsigned Object::refCnt() const {return m_pyObj ? Py_REFCNT(m_pyObj) : 0;}

   Object Object::MakeFromRaw(PyObject* pyObj) {
      return Object(pyObj, true);
   }

   Object Object::MakeFromBorrow(PyObject* pyObj) {
      return Object(pyObj, false);
   }

   Object& Object::operator=(const Object& o) {
      if(this != &o)
         Object(o).swap(*this);
      
      return *this;
   }

   Object& Object::operator=(Object&& o) {
      if(this != &o)
         Object(std::move(o)).swap(*this);

      return *this;
   }
} // namespace cppython