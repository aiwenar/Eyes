DEPENDENCY_LIBRARIES += glib-2.0 \
                        gtop-2.0

DEPENDENCY_INCLUDES += glib-2.0 \
                        libgtop-2.0

SOURCES	+= \
    eyes_ui.cpp \
    eyes_view.cpp \
    core.cpp

HEADERS += \
    eyes_view.h \
    bulwers.h \
    eyes_info.h \
    defines.hxx

OTHER_FILES += \
    dependences.pri

INCLUDEPATH += /usr/include/glib-2.0 \
    /usr/lib/glib-2.0/include \
    /usr/include/libgtop-2.0 \
    /usr/lib/libgtop-2.0/include

LIBDIR = /usr/lib

LIBS += $${LIBDIR}/libglib-2.0.so \
    $${LIBDIR}/libgtop-2.0.so
