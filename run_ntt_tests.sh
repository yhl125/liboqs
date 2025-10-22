#!/bin/bash

###############################################################################
# NTT API test runner script
###############################################################################

set -e  # exit on error

# Color definitions
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo ""
echo "================================================================="
echo "  liboqs NTT API test build and run"
echo "================================================================="
echo ""

# Detect platform
PLATFORM=$(uname -m)
OS=$(uname -s)

echo "Platform information:"
echo "  OS: $OS"
echo "  Architecture: $PLATFORM"
echo ""

# Compiler flags
CFLAGS="-O2 -Wall"
INCLUDES="-I./include"
INCLUDES="$INCLUDES -I./src/sig/ml_dsa/pqcrystals-dilithium-standard_ml-dsa-44_ref"
INCLUDES="$INCLUDES -I./src/sig/ml_dsa/pqcrystals-dilithium-standard_ml-dsa-65_ref"
INCLUDES="$INCLUDES -I./src/sig/ml_dsa/pqcrystals-dilithium-standard_ml-dsa-87_ref"

echo ""

# Verify liboqs build
if [ ! -d "build" ]; then
    echo -e "${YELLOW}build directory not found. Please build liboqs first:${NC}"
    echo "  mkdir build && cd build"
    echo "  cmake .."
    echo "  make"
    echo ""
    read -p "Continue? (y/N) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Compile test
echo "Compiling test program..."
echo "  gcc $CFLAGS $INCLUDES ..."

gcc $CFLAGS \
    -o test_ntt_api \
    tests/test_ntt_api.c \
    src/sig/oqs_ntt_api.c \
    src/sig/falcon_clean_ntt.c \
    $INCLUDES \
    -L./build/lib \
    -loqs \
    -lm

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Compile succeeded${NC}"
else
    echo -e "${RED}✗ Compile failed${NC}"
    exit 1
fi

echo ""
echo "================================================================="
echo "  Running tests..."
echo "================================================================="
echo ""

# Run tests
if [[ "$OS" == "Darwin" ]]; then
    # macOS: set DYLD_LIBRARY_PATH
    export DYLD_LIBRARY_PATH=./build/lib:$DYLD_LIBRARY_PATH
else
    # Linux: set LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=./build/lib:$LD_LIBRARY_PATH
fi

./test_ntt_api

TEST_RESULT=$?

echo ""
if [ $TEST_RESULT -eq 0 ]; then
    echo -e "${GREEN}================================================================="
    echo "  All tests passed!"
    echo -e "=================================================================${NC}"
else
    echo -e "${RED}================================================================="
    echo "  Some tests failed"
    echo -e "=================================================================${NC}"
fi

echo ""
echo "Generated files:"
echo "  ./test_ntt_api - test executable"
echo ""
echo "Additional commands:"
echo "  ./test_ntt_api              - rerun the tests"
echo "  rm test_ntt_api             - remove the test binary"
echo ""

exit $TEST_RESULT
