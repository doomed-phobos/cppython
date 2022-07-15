#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>

namespace cppython {
   class Application {
   public:
      Application();
      virtual ~Application();

      virtual int run() = 0;
   };

   class Object {
   public:
      Object(std::nullptr_t);
      Object(PyObject* pyObj);
      Object(const Object& o);
      Object(Object&& o);
      virtual ~Object();

      void swap(Object& o);

      bool isValid() const;
      unsigned refCnt() const;
      Object callObject();

      template<typename... Args>
      Object callMethod(const char* name, Args&&... args);

      Object& operator=(const Object& o);
      Object& operator=(Object&& o);
      explicit operator PyObject*() const {return m_pyObj;}
      operator bool() const {return isValid();}
   private:
      template<typename T>
      static Object callMethod_helper(T&& arg);

      PyObject* m_pyObj;
   };

   namespace types {
      template<typename T>
      class CType {
      public:
         CType(T&& value);
         Object toPython() const;
      private:
         T&& m_value;
      };

      template<typename T>
      class CType<std::vector<T>> {
      public:
         using container_t = std::vector<T>;

         CType(const std::initializer_list<T>& initializer_list);
         CType(container_t&& value);

         Object toPython() const;
      private:
         container_t m_value;
      };

      typedef CType<const char*> String;
      typedef CType<bool> Bool;
      template<typename T>
      using List = CType<std::vector<T>>;
   } // namespace types

   class Import : public Object {
   public:
      Object getAttr(const char* name);
      bool hasAttr(const char* name) const;

      static Import Make(const char* name, const types::List<const char*>& fromList);
   private:
      using Object::Object;
   };
} // namespace cppython

#include "cppython-inl.hpp"