# C:\DESENV\CPP\PARSER\PARSER.pro
QT -= gui
CONFIG += c++20 console
CONFIG -= app_bundle

# Define o nome do executável final
TARGET = ParserTopografia

# Inclui os caminhos base para facilitar os #include no código
INCLUDEPATH += "C:/DESENV/CPP" "../CORE/PROTOCOL/"

# Arquivos do projeto
HEADERS += ../CORE/PROTOCOL/EquipoParser.hpp
SOURCES += main.cpp

# Otimizações pesadas para processamento em massa (Release Mode)
CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS_RELEASE += -O3 -march=native
}
