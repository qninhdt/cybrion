cmake_minimum_required (VERSION 3.20)

project(jbt)

set(JBT_HEADERS
	"include/jbt/jbt.hpp"
	"include/jbt/internal.hpp"
	"include/jbt/tag.hpp"
	"include/jbt/serializer.hpp"
	"include/jbt/io.hpp"
	"include/jbt/compression.hpp"
	"include/jbt/file.hpp"
	"include/jbt/hjbt.hpp"
)

set(JBT_SOURCES
	"src/jbt.cpp"
	"src/tag.cpp"
	"src/serializer.cpp"
	"src/io.cpp"
	"src/compression.cpp"
	"src/file.cpp"
	"src/hjbt.cpp"
)

add_library(jbt
	${JBT_HEADERS}
	${JBT_SOURCES}
)

target_include_directories(jbt PUBLIC "include")

find_package(lz4 CONFIG REQUIRED)
target_link_libraries(jbt PUBLIC lz4::lz4)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET jbt PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(examples)

