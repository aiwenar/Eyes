DEPENDENCY_LIBRARIES += glib-2.0 \
                        gtop-2.0

DEPENDENCY_INCLUDES += glib-2.0 \
                        libgtop-2.0

SOURCES	+= \
    eyes_ui.cpp \
    eyes_view.cpp \
    eyes_clapper.cc \
    animation.cc \
    eyes_looker.cc \
    eyes_cfg.cc \
    core_new.cpp \
    eyes_window.cc \
    eyes_config.cc

HEADERS += \
    eyes_view.h \
    bulwers.h \
    eyes_info.h \
    defines.hxx \
    clap_animations.hxx \
    animation.hxx \
    eyes_cfg.hxx \
    core_new.hxx \
    eyes_window.hxx \
    eyes_config.hxx

OTHER_FILES += \
    README.markdown \
    config.cfg

ARCH=$$system(uname -m)
contains(ARCH, x86_64){
  message(target system arch is x64)
  LIBDIR = /usr/lib64
  LOCALLIB = /usr/local/lib64
}
contains(ARCH, i686){
  message(target system arch is x32)
  LIBDIR = /usr/lib
  LOCALLIB = /usr/local/lib
}

INCLUDEPATH += /usr/include/glib-2.0 \
    $${LIBDIR}/glib-2.0/include \
    /usr/include/libgtop-2.0 \
    $${LIBDIR}/libgtop-2.0/include \
    /usr/local/include/

LIBS += $${LIBDIR}/libglib-2.0.so \
    $${LIBDIR}/libgtop-2.0.so \
    $${LOCALLIB}/libconfig++.so