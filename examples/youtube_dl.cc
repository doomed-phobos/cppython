#include "cppython/cppython.hpp"

#include <iostream>

class App : public cppython::Application {
public:
   virtual int run() override {
      auto mod = cppython::Import::Make("youtube_dl.YoutubeDL", {"YoutubeDL"});
      if(!mod) {
         puts("Load youtube_dl.YoutubeDL failed");
         return 1;
      }

      auto ydl_class = mod.getAttr("YoutubeDL");
      if(!ydl_class) {
         puts("YoutubeDL class don't exists");
         return 1;
      }

      auto ydl = ydl_class.callObject();
      if(!ydl) {
         puts("YoutubeDL failed to instancing");
         return 1;
      }
      ydl.callMethod("extract_info", "https://www.youtube.com/watch?v=-FMcAInEWDc", true);
      // TODO : Añadir tupla para crear Object

      return 0;
   }
};

int main(int argc, char* argv[]) {
   App app;
   /*
   PyObject* ydl_object = PyObject_GetAttrString(module, "YoutubeDL");

   auto tuple = PyTuple_New(1);
   auto dict = PyDict_New();
   auto value = PyBool_FromLong(1);
   PyDict_SetItemString(dict, "quiet", value);
   PyTuple_SetItem(tuple, 0, dict);
   PyObject* ydl = PyObject_CallObject(ydl_object, tuple);           <==================     Este valor
   Py_DECREF(value);
   Py_DECREF(dict);
   Py_DECREF(tuple);

   Py_DECREF(ydl_object);
   PyObject* video_info = PyObject_CallMethod(ydl, "extract_info", "s i", "https://www.youtube.com/watch?v=-FMcAInEWDc", 0);
   if(PyErr_Occurred()) {
      PyErr_Print();
      return 1;
   }
   std::cout << PyList_CheckExact(video_info) << std::endl;

   Py_DECREF(ydl);*/
   
   // Finalizing

   return app.run();
}