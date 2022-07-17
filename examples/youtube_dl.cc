#include "cppython/cppython.hpp"

#include <iostream>

class App : public cppython::Application {
public:
   virtual int run() override {
      using namespace cppython::types;
      auto mod = cppython::load_module("youtube_dl.YoutubeDL", {String("YoutubeDL")});
      if(!mod) {
         CPPYTHON_SHOW_ERROR();
         return 1;
      }

      auto ydl_class = mod.getAttr("YoutubeDL").to<cppython::Callable>();
      if(!ydl_class) {
         CPPYTHON_SHOW_ERROR();
         return 1;
      }
      Dict d({
         {"verbose", Bool(true)},
         {"simulate", Bool(true)}});
      auto ydl = ydl_class.callObject(Tuple({d}));
      if(!ydl) {
         CPPYTHON_SHOW_ERROR();
         return 1;
      }

      auto video_info = ydl.callMethod("extract_info",
         Tuple{String("https://www.youtube.com/watch?v=-FMcAInEWDc"), Bool(false)}).to<Dict>();
      if(!video_info) {
         CPPYTHON_SHOW_ERROR();
         return 1;
      }

      auto format = video_info.getItem("requested_formats").to<Tuple>().getItem(0).to<Dict>();
      if(!format) {
         CPPYTHON_SHOW_ERROR();
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