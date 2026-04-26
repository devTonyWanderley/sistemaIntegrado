# C:/DESENV/CPP/LIB/TIP/CMakeLists.txt

cmake_minimum_required(VERSION 3.16)
project(LibTipos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES
    tipPonto.h
    dicionarioTextos.h
    #tipArquivo.h
)

add_library(LibTipos INTERFACE)

target_sources(LibTipos INTERFACE
    ${HEADER_FILES}
)

target_include_directories(LibTipos INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
