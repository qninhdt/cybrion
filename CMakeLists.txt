cmake_minimum_required (VERSION 3.8)

project ("cybrion")

if (MSVC)
    add_compile_options("/MP")
endif()

add_executable(${PROJECT_NAME}
    "src/main.cpp"  "src/core/log.cpp" "src/client/resource/shader_manager.cpp" "src/client/client_game.cpp" "src/util/file.cpp" "src/client/application.cpp" "src/client/GL/mesh.cpp" "src/client/GL/texture_array.cpp" "src/client/transform.cpp" "src/client/camera.cpp" "src/client/frame_profiler.cpp" "src/client/debug_screen.cpp" "src/world/chunk/chunk.cpp" "src/util/math.hpp" "src/core/registry.cpp" "src/world/world.cpp" "src/client/resource/block_loader.cpp" "src/game.cpp" "src/world/block/block.cpp" "src/client/graphic/block_renderer.cpp" "src/client/graphic/world_renderer.cpp" "src/client/stopwatch.cpp" "src/world/block/block_registry.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC src)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.hpp)

add_subdirectory(third_party/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(third_party/stb_image)
target_link_libraries(${PROJECT_NAME} PUBLIC stb_image)

find_package(robin_hood CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC robin_hood::robin_hood)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)

find_package(eventpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE eventpp::eventpp)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

add_subdirectory(third_party/nanobench)
target_link_libraries(${PROJECT_NAME} PUBLIC nanobench)
