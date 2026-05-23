cmake_minimum_required(VERSION 3.10.0)
project(snes-amp VERSION 0.1.0 LANGUAGES C)

# Release build by default
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SNES_AMP_SOURCES CONFIGURE_DEPENDS src/*.c)

file(GLOB_RECURSE SNES_AMP_HEADERS CONFIGURE_DEPENDS src/*.h)

add_executable(snes-amp ${SNES_AMP_SOURCES} ${SNES_AMP_HEADERS})

target_compile_options(snes-amp PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -g3 -O0)

target_link_options(snes-amp PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -g3 -O0)
