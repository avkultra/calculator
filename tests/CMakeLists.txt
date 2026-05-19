set(TEST_SOURCES
    test_calculator.cpp
    test_command_parser.cpp
    test_app.cpp
    ${CMAKE_SOURCE_DIR}/src/calculator.cpp
    ${CMAKE_SOURCE_DIR}/src/command_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/app.cpp
)

add_executable(calculator_tests ${TEST_SOURCES})

target_link_libraries(calculator_tests
    PRIVATE
        math
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        gtest_main
)

target_include_directories(calculator_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include
        ${CMAKE_BINARY_DIR}/_deps/googletest-src/googlemock/include
)

target_include_directories(calculator_tests
    PUBLIC
        ${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include
)

add_test(NAME calculator_tests COMMAND calculator_tests)
