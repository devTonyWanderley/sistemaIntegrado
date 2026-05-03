#	vMSerial.pro
TARGET = vMSerial_Teste
TEMPLATE = app
CONFIG += c++20 console

INCLUDEPATH += ../../CORE/TYPES ../../CORE/DRIVER

HEADERS += \
    ../../CORE/DRIVER/SerialModerno.h \
    ../../CORE/TYPES/DeviceConfigs.h

SOURCES += \
    main.cpp \
    ../../CORE/DRIVER/SerialModerno.cpp

LIBS += -lkernel32
