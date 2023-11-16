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

# -------------------------------------------

add_executable(encoderTest
    encoderTest.cpp
)

target_link_libraries(encoderTest
    pico_stdlib
    pico_encoder
)

pico_add_extra_outputs(encoderTest)

pico_enable_stdio_usb(encoderTest 1)
pico_enable_stdio_uart(encoderTest 0)