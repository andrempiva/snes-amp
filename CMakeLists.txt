cmake_minimum_required(VERSION 3.14)
project(snes-amp VERSION 0.1.0 LANGUAGES C)

# Default to Debug during development
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Main sources
file(GLOB_RECURSE SNES_AMP_SOURCES CONFIGURE_DEPENDS
    src/*.c src/lib/*.c src/cpu/*.c src/ppu/*.c src/apu/*.c
)

add_executable(snes-amp ${SNES_AMP_SOURCES})

target_include_directories(snes-amp PRIVATE src)

target_compile_options(snes-amp PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow -Wformat=2
    -Wcast-align -Wconversion -Wsign-conversion
    -Wnull-dereference -Wundef
    -g3
)

# ==================== Tests ====================

add_library(munit STATIC munit/munit.c)
target_include_directories(munit PUBLIC munit)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.c)
list(APPEND TEST_SOURCES ${SNES_AMP_SOURCES})
list(REMOVE_ITEM TEST_SOURCES ${CMAKE_SOURCE_DIR}/src/main.c)

add_executable(tests ${TEST_SOURCES})

target_include_directories(tests PRIVATE src munit)
target_link_libraries(tests PRIVATE munit)

# enable_testing()

# add_test(NAME AllTests COMMAND tests)
# add_test(NAME RomLoaderTests COMMAND tests --filter=rom_loader)
