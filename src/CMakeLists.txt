set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES
    triangle.cpp
    mesh.cpp
    vector.cpp
    display.cpp
    main.cpp
)

add_executable(${BINARY}_run ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES})

message(CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}")
message(ROOT_DIR="${ROOT_DIR}")

IF (WIN32)
target_compile_options(${BINARY}_run PRIVATE -W4 -WX)
ELSE()
target_compile_options(${BINARY}_run PRIVATE -Wall -Wextra -Wpedantic -Werror)
ENDIF()

target_include_directories(${BINARY}_run PUBLIC ${CMAKE_BINARY_DIR}/../lib/SDL/include)
target_include_directories(${BINARY}_lib PUBLIC ${CMAKE_BINARY_DIR}/../lib/SDL/include)

target_link_libraries(${BINARY}_run PRIVATE SDL3::SDL3)
target_link_libraries(${BINARY}_lib PRIVATE SDL3::SDL3)
