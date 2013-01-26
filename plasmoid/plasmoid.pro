TEMPLATE = lib
TARGET = eyes_plasmoid

OTHER_FILES += \
    plasma-applet-eyes.desktop

HEADERS += \
    eyesplasmoid.hh

SOURCES += \
    eyesplasmoid.cc

INCLUDEPATH = ../libeyes
LIBS = -L../libeyes -leyes

QT += kde
