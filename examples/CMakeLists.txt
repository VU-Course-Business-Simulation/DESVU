cmake_minimum_required(VERSION 3.15)

# Helper function to create example executables
function(add_desvu_example EXAMPLE_NAME)
  add_executable(${EXAMPLE_NAME} ${EXAMPLE_NAME}.cpp)
  target_link_libraries(${EXAMPLE_NAME} PRIVATE desvu)
  target_compile_features(${EXAMPLE_NAME} PRIVATE cxx_std_17)
endfunction()

# Create example executable
add_desvu_example(simple_queue)

