# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Projeto_UART C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
file(GLOB LIBSOURCES "lib/*.c")
file(GLOB ASSETSOURCES "animation/*.c")
file(GLOB INCSOURCES "inc/*.c")

add_executable(Projeto_UART Projeto_UART.c ${LIBSOURCES} ${ASSETSOURCES} ${INCSOURCES} inc/ssd1306_i2c.c)


# Generate PIO header
pico_generate_pio_header(Projeto_UART ${CMAKE_CURRENT_LIST_DIR}/lib/ws2818b.pio)

pico_set_program_name(Projeto_UART "Projeto_UART")
pico_set_program_version(Projeto_UART "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Projeto_UART 1)
pico_enable_stdio_usb(Projeto_UART 1)

# Add the standard library to the build
target_link_libraries(Projeto_UART
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(Projeto_UART PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Projeto_UART
        hardware_pio
        hardware_clocks
        hardware_i2c
        hardware_spi
        )

pico_add_extra_outputs(Projeto_UART)

