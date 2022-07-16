#pragma once

namespace cppython {
   class Application {
   public:
      Application();
      virtual ~Application();

      virtual int run() = 0;
   };
} // namespace cppython