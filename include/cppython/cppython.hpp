#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <tuple>

namespace cppython {
   namespace types {
      class Tuple;
   } // namespace types

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
      Object callObject(const types::Tuple& args);

      template<typename... Args>
      Object callMethod(const char* name, Args&&... args);

      Object& operator=(const Object& o);
      Object& operator=(Object&& o);
      operator PyObject*() const {return m_pyObj;}
      operator bool() const {return isValid();}
   private:
      PyObject* m_pyObj;
   };

   namespace types {
      template<typename T>
      class CType {
      public:
         explicit CType(const T& value);

         Object pyObj() const;
      private:
         T m_value;
      };

      template<typename T>
      struct is_ctype : std::false_type {};
      template<typename T>
      struct is_ctype<CType<T>> : std::true_type {};
      template<typename T>
      inline constexpr bool is_ctype_v = is_ctype<T>::value;

      template<typename T,
         std::enable_if_t<is_ctype_v<T>, int> = 0>
      class List {
      public:
         List(const std::initializer_list<T>& initializer_list) :
            List(std::vector<T>(initializer_list.begin(), initializer_list.end())) {}
         List(const std::vector<T>& container) :
            m_obj(PyList_New(0)) {
               for(auto& item : container)
                  PyList_Append(m_obj, item.pyObj());
            }

         Object pyObj() const {return m_obj;}
      private:
         Object m_obj;
      };

      class Tuple {
      public:
         template<typename... Args>
         Tuple(Args&&... args);

         Object pyObj() const;
      private:
         Object m_obj;
      };

      class Dict {
      public:
         Dict();

         template<typename T,
            std::enable_if_t<is_ctype_v<T>, int> = 0>
         void addItem(const char* key, T&& value);

         Object pyObj() const;
      private:
         Object m_obj;
      };

      typedef CType<const char*> String;
      typedef CType<bool> Bool;
   } // namespace types

   class Import : public Object {
   public:
      Object getAttr(const char* name);
      bool hasAttr(const char* name) const;

      static Import Make(const char* name, const types::List<types::String>& fromList);
   private:
      using Object::Object;
   };
} // namespace cppython

#include "cppython-inl.hpp"