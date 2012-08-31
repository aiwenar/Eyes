DEPENDENCY_LIBRARIES += \ 
  glib-2.0 \
  gtop-2.0

DEPENDENCY_INCLUDES += \ 
  glib-2.0 \
  libgtop-2.0

SOURCES += \ 
  main.cc \
  eyes.cc \
  clapper.cc\
  animation.cc\
  looker.cc\
  window.cc\
  core.cpp\
  configuration.cc \
    memory.cc \
    hardware.cc \
    cdbg.cc \
    ostream.cc \
    eyes_menu.cc \
    camera.cpp \
    hungarian.cpp \
    connection.cc

HEADERS += \ 
  defines.hxx\
  animation.hxx\
  window.hxx\
  debug.hxx\
  eyes.hxx\
  core.hxx\
  configuration.hxx \
  memory.hxx \
  cdbg.hxx \
  hardware.hxx \
  ostream.hh \
    eyes_menu.hh \
    camera.hxx \
    hungarian.h \
    connection.hxx

OTHER_FILES += \ 
  README.markdown\
  color.cfg\
  .gitignore

QT += network

system(./scripts/checklibs)

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += \
  $$system(./scripts/libcfg --cflags glib-2.0) \
  $$system(./scripts/libcfg --cflags libgtop-2.0) \
  $$system(./scripts/libcfg --cflags libconfig++) \
  $$system(./scripts/libcfg --cflags opencv)

LIBS += \
  $$system(./scripts/libcfg --libs glib-2.0) \
  $$system(./scripts/libcfg --libs libgtop-2.0) \
  $$system(./scripts/libcfg --libs libconfig++) \
  $$system(./scripts/libcfg --libs opencv) \
  -lrt

system ( echo "Eyes configured." )
system ( echo "  type 'make' to build Eyes." )













