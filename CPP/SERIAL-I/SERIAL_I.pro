#	SERIAL.pro .. nome desse arquivo
TARGET = comunica
TEMPLATE = app
CONFIG += c++20
QT += widgets

INCLUDEPATH += ../CORE/TYPES ../CORE/DRIVER ../UI

HEADERS += \
#    ../CORE/TYPES/Equipamentos.h \
    ../CORE/TYPES/configSerial.hpp \
    ../CORE/DRIVER/motorSerial.hpp \
    ../UI/GUISerial.hpp

SOURCES += \
main.cpp \
    ../UI/GUISerial.cpp \
    ../CORE/DRIVER/motorSerial.cpp \