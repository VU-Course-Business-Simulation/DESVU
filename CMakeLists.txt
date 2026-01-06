cmake_minimum_required(VERSION 3.15)
project(desvu VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Header-only library target
add_library(desvu INTERFACE)
target_include_directories(desvu INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(desvu INTERFACE cxx_std_17)

# Optionally build tests
option(DESVU_BUILD_TESTS "Build tests" ON)
if(DESVU_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Optionally build examples
option(DESVU_BUILD_EXAMPLES "Build examples" ON)
if(DESVU_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
