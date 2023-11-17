add_executable(pico_rover
    main.cpp 
)

# Need to generate UF2 file for upload to RP2040
pico_add_extra_outputs(pico_rover)

# enable usb output, disable uart output
pico_enable_stdio_usb(pico_rover 1)
pico_enable_stdio_uart(pico_rover 0)

target_link_libraries(pico_rover
    pico_stdlib
    pico_motor
    pico_encoder
)  

target_include_directories(pico_rover
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)