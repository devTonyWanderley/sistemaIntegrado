#-------------------------------------------------
# Projeto de Validação Automatizada - MotorSerial
#-------------------------------------------------

QT       += core
QT       -= gui  # Teste de console para maior agilidade

TARGET = tstMotorSerial
CONFIG += console c++20
CONFIG -= app_bundle

TEMPLATE = app

# Definições de caminhos relativos para os arquivos originais
# Ajuste o número de "../" se a estrutura de pastas for diferente
#PATH_CORE = ../../SERIAL/CORE/

INCLUDEPATH += . \
               ../../CORE/TYPES \
               ../../CORE/DRIVER

# Bibliotecas do Windows necessárias para WinAPI (Serial e QueryDosDevice)
LIBS += -lkernel32 -luser32

# Arquivos de Cabeçalho (Headers)
HEADERS += \
    ../../CORE/TYPES/configSerial.hpp \
    ../../CORE/DRIVER/motorSerial.hpp

# Arquivos de Implementação (Sources)
SOURCES += \
    main.cpp \
    ../../CORE/DRIVER/motorSerial.cpp

# Definição de local de build para não poluir a pasta de código
DESTDIR = bin
OBJECTS_DIR = obj
