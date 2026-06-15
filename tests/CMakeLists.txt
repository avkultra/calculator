find_path(POSTGRES_INCLUDE_DIR
    NAMES libpq-fe.h
    PATHS /usr/include/postgresql /usr/include
)

find_library(POSTGRES_LIBRARY
    NAMES pq
    PATHS /usr/lib/x86_64-linux-gnu /usr/lib
)

if(POSTGRES_INCLUDE_DIR AND POSTGRES_LIBRARY)
    message(STATUS "Tests: Found PostgreSQL: ${POSTGRES_INCLUDE_DIR}")
    set(HAVE_POSTGRES ON)
else()
    message(WARNING "Tests: PostgreSQL not found")
    set(HAVE_POSTGRES OFF)
endif()

set(TEST_SOURCES
    test_calculator.cpp
    test_command_parser.cpp
    test_app.cpp
    ${CMAKE_SOURCE_DIR}/src/calculator.cpp
    ${CMAKE_SOURCE_DIR}/src/command_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/app.cpp
    ${CMAKE_SOURCE_DIR}/src/cache.cpp
    ${CMAKE_SOURCE_DIR}/src/DB/databasemanager.cpp
    ${CMAKE_SOURCE_DIR}/src/DB/databaseconnection.cpp
    ${CMAKE_SOURCE_DIR}/src/DB/databaseresult.cpp
    ${CMAKE_SOURCE_DIR}/src/DB/databasestatement.cpp
)

add_executable(calculator_tests ${TEST_SOURCES})

# Базовые библиотеки
target_link_libraries(calculator_tests
    PRIVATE
        math
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        gtest_main
)

if(HAVE_POSTGRES)
    target_link_libraries(calculator_tests PRIVATE ${POSTGRES_LIBRARY})
    target_include_directories(calculator_tests PRIVATE ${POSTGRES_INCLUDE_DIR})
endif()

target_include_directories(calculator_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include
)

add_test(NAME calculator_tests COMMAND calculator_tests)
