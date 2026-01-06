cmake_minimum_required(VERSION 3.15)

# ============================================================================
# Helper Functions for Adding Examples
# ============================================================================

# Simple single-file example
# Usage: add_desvu_simple_example(example_name)
# This creates an executable from a single .cpp file
function(add_desvu_simple_example EXAMPLE_NAME)
  add_executable(${EXAMPLE_NAME} ${EXAMPLE_NAME}.cpp)
  target_link_libraries(${EXAMPLE_NAME} PRIVATE desvu)
  target_compile_features(${EXAMPLE_NAME} PRIVATE cxx_std_17)
endfunction()

# Complex multi-file example in subdirectory
# Usage: add_desvu_subdirectory_example(subdirectory_name)
# This adds a subdirectory that contains its own CMakeLists.txt
# The subdirectory should create an executable with the same name
function(add_desvu_subdirectory_example SUBDIR_NAME)
  add_subdirectory(${SUBDIR_NAME})
endfunction()

# ============================================================================
# Add Examples
# ============================================================================

# Multi-file structured example (M/M/1 queue)
add_desvu_subdirectory_example(simple_queue)

# ============================================================================
# Instructions for Adding New Examples
# ============================================================================
#
# OPTION 1: Simple single-file example
# ------------------------------------
# 1. Create a new .cpp file in the examples/ directory (e.g., my_example.cpp)
# 2. Add this line here:
#    add_desvu_simple_example(my_example)
#
# Example:
#   add_desvu_simple_example(basic_simulation)
#
# OPTION 2: Complex multi-file example
# -------------------------------------
# 1. Create a subdirectory in examples/ (e.g., examples/my_complex_example/)
# 2. Add your .h and .cpp files to that subdirectory
# 3. Create a CMakeLists.txt in the subdirectory with:
#    ```
#    add_executable(my_complex_example
#        main.cpp
#        MyClass1.cpp
#        MyClass2.cpp
#    )
#    target_include_directories(my_complex_example PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}
#    )
#    target_link_libraries(my_complex_example PRIVATE desvu)
#    ```
# 4. Add this line here:
#    add_desvu_subdirectory_example(my_complex_example)
#
# Example:
#   add_desvu_subdirectory_example(machine_shop)
#
# ============================================================================

