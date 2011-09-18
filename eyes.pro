DEPENDENCY_LIBRARIES += \ 
  glib-2.0 \
  gtop-2.0

DEPENDENCY_INCLUDES += \ 
  glib-2.0 \
  libgtop-2.0

SOURCES += \ 
  eyes_ui.cpp\
  eyes_view.cpp\
  eyes_clapper.cc\
  animation.cc\
  eyes_looker.cc\
  eyes_cfg.cc\
  eyes_window.cc\
  eyes_config.cc\
  econfig.cc\
  core.cpp\
  configurator.cc

HEADERS += \ 
  eyes_view.h\
  bulwers.h\
  eyes_info.h\
  defines.hxx\
  clap_animations.hxx\
  animation.hxx\
  eyes_cfg.hxx\
  eyes_window.hxx\
  eyes_config.hxx\
  econfig.hxx\
  debug.hxx\
  eyes.hxx\
  core.hxx\
  configurator.hxx

OTHER_FILES += \ 
  README.markdown\
  config.cfg\
  color.cfg\
  configure

system(./scripts/checklibs)

INCLUDEPATH += \
  $$system(./scripts/libcfg --cflags glib-2.0) \
  $$system(./scripts/libcfg --cflags libgtop-2.0) \
  $$system(./scripts/libcfg --cflags libconfig++)

LIBS += \
  $$system(./scripts/libcfg --libs glib-2.0) \
  $$system(./scripts/libcfg --libs libgtop-2.0) \
  $$system(./scripts/libcfg --libs libconfig++)

