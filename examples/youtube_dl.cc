#include "cppython/cppython.hpp"

#include <iostream>

class App : public cppython::Application {
public:
   virtual int run() override {
      using namespace cppython::types;
      auto mod = cppython::Import::Make("youtube_dl.YoutubeDL", {String("YoutubeDL")});
      if(!mod) {
         puts("Load youtube_dl.YoutubeDL failed");
         return 1;
      }

      auto ydl_class = mod.getAttr("YoutubeDL");
      if(!ydl_class) {
         puts("YoutubeDL class don't exists");
         return 1;
      }

      Dict d;
      d.addItem("verbose", Bool(true));
      Tuple t(d);
      auto ydl = ydl_class.callObject(t);
      if(!ydl) {
         puts("YoutubeDL failed to instancing");
         return 1;
      }
      ydl.callMethod("extract_info", String("https://www.youtube.com/watch?v=-FMcAInEWDc"), Bool(false));

      return 0;
   }
};

int main(int argc, char* argv[]) {
   App app;
   return app.run();
}