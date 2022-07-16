#include "cppython/object.hpp"

#include <algorithm>

namespace cppython {
   Object::Object(PyObject* pyObj) : m_pyObj(pyObj) {}
   Object::Object(const Object& o) {
      m_pyObj = o.m_pyObj;
      Py_XINCREF(m_pyObj);
   }
   Object::Object(Object&& o) : m_pyObj(std::exchange(o.m_pyObj, nullptr)) {}
   Object::~Object() {
      Py_XDECREF(m_pyObj);
      m_pyObj = nullptr;
   }

   void Object::swap(Object& o) {
      std::swap(m_pyObj, o.m_pyObj);
   }

   bool Object::isValid() const {return m_pyObj;}
   unsigned Object::refCnt() const {return m_pyObj ? Py_REFCNT(m_pyObj) : 0;}
   Object Object::callObject() {
      return PyObject_CallObject(*this, nullptr);
   }
   Object Object::callObject(const types::Tuple& args) {
      return PyObject_CallObject(*this, args.obj());
   }

   Object& Object::operator=(const Object& o) {
      if(this != &o)
         Object(o.m_pyObj).swap(*this);
      
      return *this;
   }

   Object& Object::operator=(Object&& o) {
      if(this != &o)
         Object(std::move(o)).swap(*this);

      return *this;
   }
} // namespace cppython