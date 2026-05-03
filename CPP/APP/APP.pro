#	APP.pro
TARGET = ECApp
TEMPLATE = app
CONFIG += c++20
QT += widgets

INCLUDEPATH += ../CORE/TYPES ../CORE/DRIVER

HEADERS += \
    ../CORE/TYPES/DeviceConfigs.h \
    ../CORE/DRIVER/SerialWin32.h \
    ../CORE/TYPES/Equipamentos.h \
    ../UI/IFSerial.h

SOURCES += \
    ../UI/IFSerial.cpp \
    main.cpp \
    ../CORE/DRIVER/SerialWin32.cpp
