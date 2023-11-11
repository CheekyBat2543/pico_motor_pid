add_executable(pico_motor_pid
    main.cpp
)

target_link_libraries(pico_motor_pid
    pico_stdlib
    pico_servo
    pico_motor
)  

# enable usb output, disable uart output
pico_enable_stdio_usb(pico_motor_pid 1)
pico_enable_stdio_uart(pico_motor_pid 0)

# Need to generate UF2 file for upload to RP2040
pico_add_extra_outputs(pico_motor_pid)