BASEDIR=$(dirname $0)

cmake \
    -G "Ninja" \
    -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_CXX_COMPILER=g++ \
    -B${BASEDIR}/out
    
cmake --build out --config Release

./out/cybrion