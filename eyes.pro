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
  configurator.cc \
  configuration.cc \
    memory.cc

HEADERS += \ 
  defines.hxx\
  animation.hxx\
  window.hxx\
  debug.hxx\
  eyes.hxx\
  core.hxx\
  configurator.hxx \
  configuration.hxx \
    memory.hxx

OTHER_FILES += \ 
  README.markdown\
  config.cfg\
  color.cfg\
  configure

system(./scripts/checklibs)

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += \
  $$system(./scripts/libcfg --cflags glib-2.0) \
  $$system(./scripts/libcfg --cflags libgtop-2.0) \
  $$system(./scripts/libcfg --cflags libconfig++)

LIBS += \
  $$system(./scripts/libcfg --libs glib-2.0) \
  $$system(./scripts/libcfg --libs libgtop-2.0) \
  $$system(./scripts/libcfg --libs libconfig++) \
  -lrt




