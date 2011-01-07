SHELL	:=	/bin/bash
GXX	:=	g++
UNI_F	:=	-I/usr/include/qt4 -lQtCore
C_CF	:=	$(UNI_F) -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/libgtop-2.0 -I/usr/lib/libgtop-2.0/include -lglib-2.0 -lgtop-2.0
EU_CF	:=	$(UNI_F)

CORE: CORE.cpp
	$(GXX) -o $@ CORE.cpp $(C_CF)

eyes_ui.o:	eyes_ui.cpp
	$(GXX) -o $@ eyes_ui.cpp $(EU_CF)
