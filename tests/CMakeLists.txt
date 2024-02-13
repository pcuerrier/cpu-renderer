set(BINARY ${CMAKE_PROJECT_NAME}_tests)

add_executable(${BINARY}
    main.cpp
)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_include_directories(${BINARY} PUBLIC ${CMAKE_BINARY_DIR}/../lib/googletest/googletest/include)

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)
