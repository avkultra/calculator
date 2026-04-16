cmake_minimum_required(VERSION 3.10)

project(calculator)


add_executable(calculator
    src/app.cpp
    src/main.cpp
)

target_link_libraries(calculator
    PRIVATE
        math
)

install(TARGETS calculator
    RUNTIME DESTINATION /usr/local/bin
)
