cmake_minimum_required(VERSION 3.14)
project(autoclicker)

set(CMAKE_C_COMPILER)

add_compile_definitions(PROGRAM_TITLE="SuperAutoClicker")
add_compile_definitions(PROGRAM_VERSION="1.0.0")

add_executable(autoclicker main.c
        beep.c beep.h
        click.c click.h)

target_include_directories(autoclicker
        PRIVATE
          ${PROJECT_SOURCE_DIR}/include)