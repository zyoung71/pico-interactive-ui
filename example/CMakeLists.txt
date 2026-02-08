cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(PICO_BOARD pico)
set(PICO_PLATFORM rp2040)

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

pico_sdk_init()

project(InteractiveUI C CXX ASM)

add_subdirectory(../ build)

add_executable(InteractiveUI
    src/main.cpp
)

target_include_directories(InteractiveUI PUBLIC
    include
)

target_link_libraries(InteractiveUI
    pico_stdlib
    pico-interactive-ui
)

pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 1)
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})