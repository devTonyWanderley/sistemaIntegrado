#	EticaConstrutora.pro
# Nome do programa
TARGET = ECApp
TEMPLATE = app

# Ativando C++20 para o MinGW
CONFIG += c++20

# O Qt precisa do módulo widgets para janelas
QT += widgets

# Por enquanto, listamos os arquivos sem caminhos de diretório
# Você vai adicionando nomes aqui conforme for criando os arquivos
INCLUDEPATH += CORE/TYPES CORE/DRIVER
SOURCES += main.cpp \
    CORE/DRIVER/SerialWin32.cpp
HEADERS +=  \
    CORE/DRIVER/SerialWin32.h \
    CORE/TYPES/DeviceConfigs.h \
    CORE/TYPES/Dicionario.h \
    CORE/TYPES/tipPonto.h
