cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(hex2net LANGUAGES C)

add_executable(hex2net hex2net.c)

target_compile_options(hex2net PRIVATE -Wall -Wextra -Wformat=2 -Wunused -Wunreachable-code
				-Wlogical-op -Wshadow -Wpacked -Wswitch-default -Wswitch-enum
				-Wmissing-format-attribute -Wpointer-arith -Wcast-align
				-Wcast-qual -Wvla -Wfloat-equal -Wredundant-decls)

install(TARGETS hex2net RUNTIME DESTINATION .)
