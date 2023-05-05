add_library(irrKlang INTERFACE)

if (WIN32)
    set(IRRKLANG_LIB "${CMAKE_SOURCE_DIR}/third_party/irrKlang/bin/libIrrKlang.lib")
elseif(LINUX AND NOT APPLE)
    set(IRRKLANG_LIB "${CMAKE_SOURCE_DIR}/third_party/irrKlang/bin/libIrrKlang.so")
endif()

target_link_libraries(irrKlang INTERFACE ${IRRKLANG_LIB})
target_include_directories(irrKlang INTERFACE include)