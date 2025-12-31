cmake_minimum_required(VERSION 3.15)

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.1
)
FetchContent_MakeAvailable(Catch2)

# Enable testing
enable_testing()

# Helper function to create test executables
function(add_desvu_test TEST_NAME)
  add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
  target_link_libraries(${TEST_NAME} PRIVATE desvu Catch2::Catch2WithMain)
  target_compile_features(${TEST_NAME} PRIVATE cxx_std_17)

  # Add test to CTest
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

# Create test executables
add_desvu_test(test_simulator)
add_desvu_test(test_event_stats)
add_desvu_test(test_time_weighted_stats)
add_desvu_test(test_stats_collector)

