cmake_minimum_required(VERSION 3.10)

project(calculator VERSION 1.3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build unit tests" ON)
option(ENABLE_CLANG_FORMAT "Enable clang-format" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy" ON)

# ПОИСК POSTGRESQL
find_path(POSTGRES_INCLUDE_DIR
    NAMES libpq-fe.h
    PATHS /usr/include/postgresql /usr/include
)

find_library(POSTGRES_LIBRARY
    NAMES pq
    PATHS /usr/lib/x86_64-linux-gnu /usr/lib
)

if(POSTGRES_INCLUDE_DIR AND POSTGRES_LIBRARY)
    message(STATUS "Found PostgreSQL: ${POSTGRES_INCLUDE_DIR}")
    set(HAVE_POSTGRES ON)
else()
    message(WARNING "PostgreSQL not found. Database features disabled.")
    set(HAVE_POSTGRES OFF)
endif()

# INCLUDE ДИРЕКТОРИИ (глобальные)
include_directories(/usr/local/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

if(HAVE_POSTGRES)
    include_directories(${POSTGRES_INCLUDE_DIR})
endif()

# FETCH
include(FetchContent)

FetchContent_Declare(
    libmath
    GIT_REPOSITORY  https://github.com/avkultra/libmath.git
    GIT_TAG main
)
FetchContent_MakeAvailable(libmath)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

# nlohmann/json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

add_executable(calculator
    src/app.cpp
    src/main.cpp
    src/logger.cpp
    src/command_parser.cpp
    src/calculator.cpp
    src/cache.cpp
    src/DB/databaseconnection.cpp
    src/DB/databaseresult.cpp
    src/DB/databasestatement.cpp
    src/DB/databasemanager.cpp
)

target_link_libraries(calculator
    PRIVATE
        math
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

# PostgreSQL линковка (тоже ПОСЛЕ создания таргета)
if(HAVE_POSTGRES)
    target_link_libraries(calculator PRIVATE ${POSTGRES_LIBRARY})
endif()


if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_PATH NAMES clang-format-19 clang-format REQUIRED)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_PATH}")

    file(GLOB_RECURSE CLANG_FORMAT_SRC CONFIGURE_DEPENDS
        include/*.h
        src/*.cpp
    )

    add_custom_target(calculator-format
        COMMAND ${CLANG_FORMAT_PATH} -style=file -i ${CLANG_FORMAT_SRC}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format on calculator project"
    )
endif()

# INSTALL
install(TARGETS calculator
    RUNTIME DESTINATION /usr/local/bin
)

# TESTS
if(BUILD_TESTS)
    enable_testing()
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(tests)
endif()
