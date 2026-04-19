cmake_minimum_required(VERSION 3.10)

project(calculator)

include(FetchContent)

FetchContent_Declare(
    libmath
    GIT_REPOSITORY git@github.com:avkultra/libmath.git
    GIT_TAG main
)

FetchContent_MakeAvailable(libmath)

add_executable(calculator
    src/app.cpp
    src/main.cpp
)

target_link_libraries(calculator
    PRIVATE
        math
)

option(ENABLE_CLANG_FORMAT "Enable clang-format" ON)
if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_PATH NAMES clang-format-19 clang-format REQUIRED)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_PATH}")

    file(GLOB_RECURSE CLANG_FORMAT_SRC CONFIGURE_DEPENDS
	include/*.h
	src/*.cpp
    )

add_custom_target(clang-format-target
       COMMAND
	   ${CLANG_FORMAT_PATH} -style=file -i ${CLANG_FORMAT_SRC}
       WORKING_DIRECTORY
	   ${CMAKE_SOURCE_DIR}
       COMMENT
	   "Formatting ${CLANG_FORMAT_DEMO} with ${CLANG_FORMAT_PATH} ..."
   )

   add_dependencies(math clang-format-target)
endif()


option(ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_PATH NAMES clang-tidy-19 calng-tidy REQUIRED)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_PATH}")

    set(CLANG_TIDY_COMMAND ${CLANG_TIDY_PATH} "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
    set_target_properties(math PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

install(TARGETS calculator
    RUNTIME DESTINATION /usr/local/bin
)
