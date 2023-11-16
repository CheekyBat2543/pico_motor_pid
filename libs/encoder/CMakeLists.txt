add_library(pico_encoder
    INTERFACE
        encoder.h
)

target_link_libraries(pico_encoder
    INTERFACE
        pico_stdlib
)   

target_include_directories(pico_encoder
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)