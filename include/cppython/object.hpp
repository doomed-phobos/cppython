#pragma once
#include <type_traits>
#include <string>

typedef struct _object PyObject;

namespace cppython {
   class Object {
   public:
      Object();
      Object(const Object& o);
      Object(Object&& o);
      virtual ~Object();
      
      void swap(Object& o);

      bool isValid() const;
      unsigned refCnt() const;
      bool hasAttr(const char* name) const;
      Object getAttr(const char* name) const;
      std::string repr() const;
      std::string str() const;

      ///
      /// Cast Object to Derived only if is the same type.
      ///
      template<typename Derived,
         std::enable_if_t<
            std::is_base_of_v<Object, Derived> && std::is_same_v<decltype(std::declval<Derived>().CheckType), bool(const Object&)>, int> = 0>
      Derived to() {
         if(!canBe<Derived>())
            return Derived();
         
         Derived d;
         d.swap(*this);
         return d;
      }

      template<typename Derived,
         std::enable_if_t<
            std::is_base_of_v<Object, Derived> && std::is_same_v<decltype(std::declval<Derived>().CheckType), bool(const Object&)>, int> = 0>
      bool canBe() {
         return isValid() && Derived::CheckType(*this);
      }
      
      ///
      /// Make a empty object
      ///
      static Object MakeNull();
      ///
      /// Create a Object and unref when calling destructor
      ///
      static Object MakeFromRaw(PyObject* pyObj);
      ///
      /// Just wrap a PyObject*, without unref when calling destructor
      ///
      static Object MakeFromBorrow(PyObject* pyObj);

      Object& operator=(const Object& o);
      Object& operator=(Object&& o);
      operator PyObject*() const {return m_pyObj;}
      operator bool() const {return isValid();}
   protected:
      Object(PyObject* pyObj, bool unref = true);
   private:
      PyObject* m_pyObj;
      bool m_unref;
   };
} // namespace cppython