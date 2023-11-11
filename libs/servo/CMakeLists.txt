add_library(pico_servo 
    STATIC 
        servo.cpp servo.h          
)

target_link_libraries(pico_servo
    PRIVATE
        hardware_pwm   
        hardware_gpio     
        hardware_clocks
)

target_include_directories(pico_servo 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
