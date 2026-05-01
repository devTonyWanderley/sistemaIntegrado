#	vSerial.pro
TARGET = vSerial_Teste
TEMPLATE = app
CONFIG += c++20 console

# Sobe duas pastas para achar o CORE (../../)
INCLUDEPATH += ../../CORE/TYPES ../../CORE/DRIVER

HEADERS += \
    ../../CORE/DRIVER/SerialWin32.h \
    ../../CORE/TYPES/DeviceConfigs.h

SOURCES += \
    main.cpp \
    ../../CORE/DRIVER/SerialWin32.cpp
