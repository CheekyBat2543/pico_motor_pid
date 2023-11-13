add_executable(servoTest 
    servoTest.cpp
)

target_link_libraries(servoTest
    pico_stdlib
    pico_motor
)

pico_add_extra_outputs(servoTest)

pico_enable_stdio_usb(servoTest 1)
pico_enable_stdio_uart(servoTest 0)

