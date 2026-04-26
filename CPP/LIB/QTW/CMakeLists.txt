# C:/DESENV/CPP/LIB/QTW/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(LibQtWidgets LANGUAGES CXX)

# 1. Ativa a automação do Qt
set(CMAKE_AUTOMOC ON) # Processa Q_OBJECT
set(CMAKE_AUTOUIC ON) # Processa arquivos .ui
set(CMAKE_AUTORCC ON) # Processa recursos (ícones/imagens)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Localiza os módulos necessários do Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 3. Define os arquivos da biblioteca
# Adicione novos Widgets aqui conforme for criando
set(QTW_SOURCES
    ConfigSerialWidget.h
    ConfigSerialWidget.cpp
    ConfigSerialWidget.ui
)

# 4. Cria a biblioteca estática
add_library(LibQtWidgets STATIC ${QTW_SOURCES})

# 5. Configura inclusão de diretórios para quem for usar esta lib
target_include_directories(LibQtWidgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(LibQtWidgets PUBLIC ../COM)

# 6. Linka com as dependências
# PUBLIC LibTipos: para que a UI conheça a struct ConfigSerial
# Qt6::Widgets: para as classes gráficas
target_link_libraries(LibQtWidgets 
    PUBLIC Qt6::Widgets 
    PUBLIC LibTipos
)
