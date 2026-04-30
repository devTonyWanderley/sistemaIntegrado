cmake_minimum_required(VERSION 3.16)
project(Projeto LANGUAGES CXX)

# Buscar os componentes do Qt 6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Habilitar o processamento automático de arquivos do Qt (.ui, signals/slots, recursos)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# Forçar C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Localização da libserialport (3RD)
set(LIBSERIALPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3RD/libserialport")

# Adiciona as subpastas
add_subdirectory(LIB/TIP)
add_subdirectory(LIB/COM)
#add_subdirectory(LIB/PRO)
add_subdirectory(LIB/GEO)
add_subdirectory(APP/TOP)
#add_subdirectory(APP/ADM)
