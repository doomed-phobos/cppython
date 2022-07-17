find_package(Python COMPONENTS Development REQUIRED)

add_library(cppython STATIC
   application.cc
   object.cc
   module.cc
   callable.cc
   instance.cc
   error.cc
   ctypes.cc)
target_include_directories(cppython PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(cppython Python::Python)
target_compile_definitions(cppython PUBLIC -DPY_SSIZE_T_CLEAN)