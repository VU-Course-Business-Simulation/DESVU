# M/M/1 Queue Simulation Example

add_executable(simple_queue
    src/main.cpp
    src/events.cpp
    src/Server.cpp
)

target_include_directories(simple_queue PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(simple_queue PRIVATE desvu)

