add_library(irrKlang INTERFACE)

set(IRRKLANG_LIB "${CMAKE_SOURCE_DIR}/third_party/irrKlang/lib/irrKlang.lib")

target_link_libraries(irrKlang INTERFACE ${IRRKLANG_LIB})
target_include_directories(irrKlang INTERFACE include)