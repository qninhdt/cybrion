cmake_minimum_required (VERSION 3.20)

project ("cybrion")

file(GLOB_RECURSE SOURCES "src/*.cpp")

if (MSVC)
	list(APPEND SOURCES config.rc)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC src)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if (CMAKE_COMPILER_IS_GNUC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -w")
endif()

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
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/third_party/irrKlang/bin/irrKlang.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)     
endif()    