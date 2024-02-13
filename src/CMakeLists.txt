set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES
    main.cpp
)

add_executable(${BINARY}_run ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES})

message(CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}")
message(ROOT_DIR="${ROOT_DIR}")

target_compile_options(${BINARY}_run PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_include_directories(${BINARY}_run PUBLIC ${CMAKE_BINARY_DIR}/../lib/SDL/include)
target_include_directories(${BINARY}_lib PUBLIC ${CMAKE_BINARY_DIR}/../lib/SDL/include)

target_link_libraries(${BINARY}_run PRIVATE SDL3::SDL3)
target_link_libraries(${BINARY}_lib PRIVATE SDL3::SDL3)
