find_package(Python COMPONENTS Development REQUIRED)

add_library(cppython STATIC
   cppython.cc)
target_include_directories(cppython PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(cppython Python::Python)