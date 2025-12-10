cmake_minimum_required(VERSION 3.10)

# Название проекта
project(lab3 C)

# Стандарт C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Доп. флаги компилятора (если gcc/clang)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Все исходники
set(SOURCES
    main.c
    arg.c
    container.c
    sort.c
    struct.c
    io.c
)

# Исполняемый файл lab3
add_executable(lab3 ${SOURCES})
