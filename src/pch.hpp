#pragma once

// STL
#include <type_traits>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <stack>
#include <queue>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <algorithm>
#include <array>
#include <ranges>
#include <chrono>
#include <thread>
#include <mutex>
#include <limits>

// third-party libraries
#include <irrKlang.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/hash.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <yaml-cpp/yaml.h>
#include <stb_image.h>
#include <stb_image_resize.h>
#include <BS_thread_pool.hpp>
#include <concurrentqueue/concurrentqueue.h>
#include <FastNoiseLite.h>
#include <jbt/jbt.hpp>
#include <jbt/hjbt.hpp>

// common files
#include "common.hpp"