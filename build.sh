BUILD_DIR=$(dirname "$(realpath $0)")/build/debug
SOURCE_DIR="$(pwd)"

if [ -d "$BUILD_DIR" ]; then rm -rf $BUILD_DIR; fi
mkdir -p $BUILD_DIR
BUILD_DIR=$BUILD_DIR

cmake -S ${SOURCE_DIR} -B ${BUILD_DIR}
cmake --build ${BUILD_DIR}

doxygen Doxyfile.conf