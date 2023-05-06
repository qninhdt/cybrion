if ! dpkg -l | grep -qw build-essential; then
    sudo apt-get install -y build-essential
fi

if ! dpkg -l | grep -qw cmake; then
    sudo apt-get install -y cmake
fi

if ! dpkg -l | grep -qw ninja-build; then
    sudo apt-get install -y ninja-build
fi

if ! dpkg -l | grep -qw libsdl2-dev; then
    sudo apt-get install -y libsdl2-dev libgl1-mesa-dev
fi

# install vcpkg & libraries
if [ ! -d "./tools/vcpkg" ] 
then
    cd tools
    git clone https://github.com/microsoft/vcpkg
    sudo ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
    sudo ./vcpkg/vcpkg install \
        glm \
        lz4 \
        robin-hood-hashing \
        spdlog \
        bshoshany-thread-pool \
        yaml-cpp \
        imgui[opengl3-binding,sdl2-binding] \
        concurrentqueue
    cd ..
fi

BASE_DIR=$(dirname "$0")
BUILD_DIR=${BASE_DIR}/build

cmake \
    -G "Ninja" \
    -DCMAKE_TOOLCHAIN_FILE=${BASE_DIR}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_BUILD_TYPE=Release \
    -B${BASE_DIR}/out \
    --no-warn-unused-cli
    
cmake --build out

# create build
mkdir -p build

# copy resources
cp -r resources ${BUILD_DIR}/resources

# copy execute file
cp -r out/cybrion build/cybrion