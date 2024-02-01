TEST_DIR=$(dirname "$(realpath $0)")/build/test
SOURCE_DIR="$(pwd)"

if [ -d "$TEST_DIR" ]; then rm -rf $TEST_DIR; fi
mkdir -p $TEST_DIR
TEST_DIR=$TEST_DIR/out

cmake -S ${SOURCE_DIR} -B ${TEST_DIR} -DUNIT_TEST=YES
cmake --build ${TEST_DIR} --target all
GTEST_COLOR=1 ctest --test-dir ${TEST_DIR} --verbose