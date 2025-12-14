cmake_minimum_required(VERSION 3.10)
project(lab3 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_compile_options(-Wall -Wextra -pedantic)

add_executable(lab3
    main.c
    arg.c
    container.c
    sort.c
    struct.c
    io.c
)

add_executable(measure
    measure.c
    container.c
    struct.c
    sort.c
)

set_target_properties(measure PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)