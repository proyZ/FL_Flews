#
# Makefile for the flews widget library and example program.
#
# This file is based on the one from the contribkit for FLTK by Michael Sweet.
#
# Permission is granted to use, copy, modify, and redistribute this file as
# needed.
#

#
# Programs...
#

AR	=	/usr/bin/ar
CC	=	gcc
CXX	=	g++
RANLIB	=	ranlib
RM	=	/bin/rm -f
SHELL	=	/bin/sh

#
# Program options...
#

ARFLAGS	=	crvs
CFLAGS	=	-g -O2 -I/usr/X11R6/include  -I./
CXXFLAGS =	-g -O2 -I/usr/X11R6/include -I./
LIBS	=	-lfltk  -lXext -lX11 -lm `fltk-config --ldflags`
LDFLAGS	=	-g -O2 -L/usr/X11R6/lib64

#
# Rules...
#

.SILENT:
.SUFFIXES:	.c .cxx .h .H .o .fl .cpp
.c.o:
	echo Compiling $<...
	$(CC) $(CFLAGS) -c $<
.cxx.o:
	echo Compiling $<...
	$(CXX) $(CXXFLAGS) -c $<

.cpp.o:
	echo Compiling $<...
	$(CXX) $(CXXFLAGS) -c $<

.fl.cxx:
	echo Compiling $<...
	fluid  -c $<

.fl.cpp:
	echo Compiling c$<...
	fluid  -c $<

#
# Make all targets...
#

all:	testflews libflews.a

#
# Remove object and target files...
#

clean:
	$(RM) *.o
	$(RM) testflews
	$(RM) libflews.a

#
# Make the flews test program.
#

testflews:	libflews.a testflews.o
	echo Linking $@...
	$(CXX) $(LDFLAGS) -o testflews testflews.o libflews.a $(LIBS)

testflews.o:	FL/Fl_flews.h 

#
# Make the flews widget library.
#

flews_OBJS=	Fl_Value_Slider_Input.o Fl_Value_Input_Spin.o Fl_Spin.o \
	Fl_Quaternion.o Fl_Ball.o Fl_Multi_Value_Slider.o 

libflews.a:	$(flews_OBJS)
	echo Building library $@...
	$(RM) libflews.a
	$(AR) $(ARFLAGS) libflews.a $(flews_OBJS)
	$(RANLIB) libflews.a

$(flews_OBJS):	FL/Fl_flews.h FL/Fl_Value_Slider_Input.H \
	FL/Fl_Value_Input_Spin.H FL/Fl_Spin.H FL/Fl_Ball.H \
	FL/Fl_Multi_Value_Slider.H FL/Fl_Quaternion.H

#
# End of Makefile.
#
