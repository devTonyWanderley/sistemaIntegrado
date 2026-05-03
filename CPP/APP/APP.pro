#	APP.pro
TARGET = ECApp
TEMPLATE = app
CONFIG += c++20
QT += widgets

INCLUDEPATH += ../CORE/TYPES ../CORE/DRIVER

HEADERS += \
    ../CORE/DRIVER/SerialModerno.h \
    ../CORE/TYPES/DeviceConfigs.h \
    ../CORE/TYPES/Equipamentos.h \
    ../UI/IFSerial.h

SOURCES += \
    ../CORE/DRIVER/SerialModerno.cpp \
    ../UI/IFSerial.cpp \
    main.cpp \
