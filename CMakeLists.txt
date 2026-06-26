cmake_minimum_required(VERSION 3.10)

project(calculator VERSION 1.4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build unit tests" ON)
option(ENABLE_CLANG_FORMAT "Enable clang-format" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy" ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

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
    src/server.cpp
    calculator.service
)

target_link_libraries(calculator
    PRIVATE
        math
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        Boost::system
        Threads::Threads
)

# PostgreSQL линковка
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

# Установка systemd unit
install(FILES ${CMAKE_SOURCE_DIR}/calculator.service
    DESTINATION /etc/systemd/system)
install(FILES calculator.service DESTINATION /etc/systemd/system)


# CPack для deb-пакета
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "calculator-service")
set(CPACK_PACKAGE_VERSION "1.4.0")
set(CPACK_PACKAGE_DESCRIPTION "Calculator service")
set(CPACK_PACKAGE_VENDOR "avkultra")
set(CPACK_PACKAGE_CONTACT "avk_job@mail.ru")
set(CPACK_PACKAGE_HOMEPAGE "https://github.com/avkultra/calculator")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "avkultra <avk_job@mail.ru>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpq5, postgresql, libboost-system-dev, libboost-thread-dev")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_amd64")


include(CPack)

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

option(ENABLE_THREAD_SANITIZER OFF)

if(ENABLE_THREAD_SANITIZER)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=thread -g -O1")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=thread")
    message(STATUS "ThreadSanitizer enabled")
endif()
