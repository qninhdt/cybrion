cmake_minimum_required (VERSION 3.26)

project ("cybrion")

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC src)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.hpp)

add_subdirectory(third_party/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(third_party/irrKlang)
target_link_libraries(${PROJECT_NAME} PUBLIC irrKlang)

add_subdirectory(third_party/stb_image)
target_link_libraries(${PROJECT_NAME} PUBLIC stb_image)
 
add_subdirectory(third_party/FastNoiseLite)
target_link_libraries(${PROJECT_NAME} PUBLIC FastNoiseLite)

add_subdirectory(third_party/jbt)
target_link_libraries(${PROJECT_NAME} PUBLIC jbt)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog spdlog::spdlog_header_only)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC yaml-cpp)

find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS})

find_package(unofficial-concurrentqueue CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::concurrentqueue::concurrentqueue)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/third_party/irrKlang/bin/irrKlang.os"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)     
endif()    