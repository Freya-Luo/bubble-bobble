add_library(hashutil yshash.cpp yshash.h)
target_link_libraries(hashutil simplebitmap)
target_include_directories(hashutil PUBLIC .)