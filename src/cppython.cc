#include "cppython/cppython.hpp"

#include <utility>

namespace cppython {
   Application::Application() {
      Py_Initialize();
      if(!Py_IsInitialized())
         throw std::runtime_error("Something is wrong at init Python");
   }

   Application::~Application() {
      Py_Finalize();
   }

   Object::Object(std::nullptr_t) : m_pyObj(nullptr) {}

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
      return PyObject_CallObject((PyObject*)*this, nullptr);
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

   bool initialize() {
      Py_Initialize();
      
      return Py_IsInitialized();
   }

   bool finalize() {
      return Py_FinalizeEx() == 0;
   }

   Object Import::getAttr(const char* name) {
      return PyObject_GetAttrString((PyObject*)*this, name);
   }
   bool Import::hasAttr(const char* name) const {
      return PyObject_HasAttrString((PyObject*)*this, name);
   }

   Import Import::Make(const char* name, const types::List<const char*>& fromList) {   
      PyObject* mod = PyImport_ImportModuleEx(name, nullptr, nullptr, (PyObject*)fromList.toPython());
      if(!mod) {
         // TODO: Invoke error callback?
         return nullptr;
      }

      return mod;
   }
} // namespace cppython