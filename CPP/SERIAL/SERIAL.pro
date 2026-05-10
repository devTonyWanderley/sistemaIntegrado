#	SERIAL.pro .. nome desse arquivo
TARGET = comSerial
TEMPLATE = app
CONFIG += c++20
QT += widgets

INCLUDEPATH += ../CORE/TYPES ../CORE/DRIVER

HEADERS += \
#    ../CORE/TYPES/Equipamentos.h \
    ../CORE/TYPES/configSerial.hpp \
 \#    ../UI/IFSerial.h
    ../CORE/DRIVER/motorSerial.hpp

SOURCES += \
 #   ../UI/IFSerial.cpp \
    main.cpp \
    ../CORE/DRIVER/motorSerial.cpp \
#   O que está comentado, está aguardando revisão.