cmake_minimum_required(VERSION 3.16)
project(TesteIntegracao LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Inclui as bibliotecas que criamos
add_subdirectory(../LIB/COM bin_com)
add_subdirectory(../LIB/TIP bin_tip)

add_executable(TesteIntegracao main.cpp)

# Linka com o motor serial e com os tipos
target_link_libraries(TesteIntegracao PRIVATE MotorComunicacao LibTipos)

# No Windows, o motor serial precisa da kernel32
target_link_libraries(TesteIntegracao PRIVATE kernel32)
