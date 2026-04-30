set(SP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../3RD/libserialport")

# 1. Cria uma biblioteca apenas para os fontes em C
add_library(sp_backend STATIC
    "${SP_DIR}/serialport.c"
    "${SP_DIR}/windows.c"
)

# Define as macros necessárias para a compilação em C
target_compile_definitions(sp_backend PRIVATE
    SP_PRIV=
    SP_API=
    LIBSERIALPORT_MS_PACKAGE_VERSION="0.1.1"
)
target_include_directories(sp_backend PUBLIC ${SP_DIR})

# 2. Sua biblioteca MotorSerial (C++)
add_library(lib_com STATIC
    motorSerial.cpp
    motorSerial.h
)

target_include_directories(lib_com PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SP_DIR}
)

# Linka a biblioteca C na sua biblioteca C++
target_link_libraries(lib_com PUBLIC sp_backend lib_tip Qt6::Core)

if(WIN32)
    target_link_libraries(lib_com PRIVATE setupapi advapi32 shell32 user32)
endif()
