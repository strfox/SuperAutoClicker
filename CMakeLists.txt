cmake_minimum_required(VERSION 3.14)
project(autoclicker)

set(MINGW)

add_compile_definitions(PROGRAM_TITLE="SuperAutoClicker")
add_compile_definitions(PROGRAM_VERSION="1.0.0")


add_executable(autoclicker main.c
        beep.c beep.h
        click.c click.h
        resources.rc)

target_include_directories(autoclicker
        PRIVATE
          ${PROJECT_SOURCE_DIR}/include)

if (MINGW)
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff <DEFINES> <INCLUDES> <FLAGS> <SOURCE> <OBJECT>")
endif(MINGW)