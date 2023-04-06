cmake_minimum_required (VERSION 3.8)

project ("cybrion")

add_executable(${PROJECT_NAME}
    "src/main.cpp"  "src/core/log.cpp" "src/client/resource/shader_manager.cpp" "src/client/local_game.cpp" "src/util/file.cpp" "src/client/application.cpp" "src/client/GL/mesh.cpp" "src/client/GL/texture_array.cpp" "src/core/transform.cpp" "src/client/camera.cpp" "src/client/frame_profiler.cpp" "src/client/hud.cpp" "src/world/chunk/chunk.cpp" "src/util/math.hpp"  "src/world/world.cpp" "src/client/resource/block_loader.cpp" "src/game.cpp" "src/world/block/block.cpp" "src/client/graphic/block_renderer.cpp" "src/client/graphic/world_renderer.cpp" "src/core/stopwatch.cpp" "src/world/block/blocks.cpp" "src/world/entity/entity.cpp" "src/player.cpp" "src/util/math.cpp" "src/client/GL/texture.cpp" "src/voxel_ray.cpp"  "src/client/graphic/basic_mesh_generator.cpp" "src/client/graphic/chunk_renderer.cpp" "src/physic/aabb.cpp" "src/world/world_generator.cpp" "src/client/graphic/entity_renderer.cpp" "src/core/pool.cpp" "third_party/FastNoiseLite/src/FastNoiseLite.cpp"  "src/world/block/building/fence_block.cpp" "src/world/block/nature/plant_block.cpp" "src/world/block/decoration/tablecloth_block.cpp" "src/world/block/nature/log_block.cpp" "src/world/block/decoration/chess_block.cpp" "src/world/block/building/fence_gate_block.cpp" "src/client/GL/framebuffer.cpp")

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

find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS})

find_package(unofficial-concurrentqueue CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::concurrentqueue::concurrentqueue)

find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/third_party/irrKlang/bin/irrKlang.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)         