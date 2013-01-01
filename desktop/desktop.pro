SOURCES += \
  main.cc \
  window.cc

HEADERS += \
  window.hxx

TARGET = ../eyes
INCLUDEPATH = ../libeyes
LIBS = -L../libeyes -leyes

INCLUDEPATH += \
  $$system(../scripts/libcfg --cflags opencv)

system ( echo "Eyes configured." )
system ( echo "  type 'make' to build Eyes." )
