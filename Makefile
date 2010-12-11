SHELL	:=	/bin/bash
GXX	:=	g++
EU_CF	:=	-I/usr/include/qt4 -LQt

eyes_ui.o:	eyes_ui.cpp
	$(GXX) -o $@ eyes_ui.cpp $(EU_CF)
