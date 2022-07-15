function(cppython_add_example NAME)
   add_executable(${NAME} ${NAME}.cc)
   target_link_libraries(${NAME} cppython)   
endfunction()

cppython_add_example(youtube_dl)