#include "cppython/cppython.hpp"

#include <iostream>
#include <Python.h>

class App : public cppython::Application {
public:
   virtual int run() override {
      using namespace cppython::types;
      auto mod = cppython::load_module("youtube_dl.YoutubeDL", {String("YoutubeDL")});
      if(!mod) {
         puts("Load youtube_dl.YoutubeDL failed");
         return 1;
      }

      auto ydl_class = mod.getAttr("YoutubeDL").to<cppython::Callable>();
      if(!ydl_class) {
         puts("YoutubeDL class don't exists");
         return 1;
      }
      Dict d({
         {"verbose", Bool(true)},
         {"simulate", Bool(true)}});
      auto ydl = ydl_class.callObject(Tuple({d}));
      if(!ydl) {
         puts("YoutubeDL failed to instancing");
         return 1;
      }

      auto video_info = ydl.callMethod("extract_info",
         Tuple{String("https://www.youtube.com/watch?v=-FMcAInEWDc"), Bool(false)}).to<Dict>();
      if(!video_info) {
         puts("Error to get video info");
         return 1;
      }

      auto format = video_info.getItem("requested_formats").to<Tuple>().getItem(0).to<Dict>();
      if(!format) {
         puts("Error to get format");
         return 1;
      }
      std::cout << format.getItem("url").to<String>().value() << std::endl;

      return 0;
   }
};

int main(int argc, char* argv[]) {
   App app;
   return app.run();
}