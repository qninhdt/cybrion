add_library(stb_image src/stb_image.cpp "include/stb_image_resize.h")

target_include_directories(stb_image PUBLIC include)