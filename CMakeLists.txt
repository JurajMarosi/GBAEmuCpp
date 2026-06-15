cmake_minimum_required(VERSION 3.20)
project(GBAEmulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(gbaEmu
    src/main.cpp
    src/core/bus.cpp
    src/core/cpu.cpp
    src/core/ppu.cpp
    src/core/cartridge.cpp
)

target_include_directories(gbaEmu PRIVATE src)