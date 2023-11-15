add_library(pico_encoder
    STATIC
        encoder.cpp
)

target_link_libraries(pico_encoder
    PRIVATE
        hardware_gpio
        hardware_clocks
)   

target_include_directories(pico_encoder
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
)