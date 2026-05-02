#	APP.pro
TARGET = ECApp
TEMPLATE = app
CONFIG += c++20
QT += widgets

# O ".." serve para subir uma pasta e achar o CORE
INCLUDEPATH += ../CORE/TYPES ../CORE/DRIVER

HEADERS += \
    ../CORE/TYPES/tipPonto.h \
    ../CORE/TYPES/Dicionario.h \
    ../CORE/TYPES/DeviceConfigs.h \
    ../CORE/DRIVER/SerialWin32.h \
    PainelConexao.h

SOURCES += \
    PainelConexao.cpp \
    main.cpp \
    ../CORE/DRIVER/SerialWin32.cpp
