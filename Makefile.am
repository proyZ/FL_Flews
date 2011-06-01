ACLOCAL_AMFLAGS = -I m4



lib_LTLIBRARIES = libflews.la

libflews_la_SOURCES = Fl_Ball.cpp\
                      Fl_Multi_Value_Slider.cpp\
                      Fl_Quaternion.cpp\
                      Fl_Spin.cpp\
                      Fl_Value_Input_Spin.cpp\
                      Fl_Value_Slider_Input.cpp\
                      gl_image.cpp

libflews_la_CXXFLAGS=@FLTK_CFLAGS@

libflews_la_LIBS=@FLTK_LIBS@


# please see info libtool -version-info :
#`-version-info CURRENT[:REVISION[:AGE]]'
#  If OUTPUT-FILE is a libtool library, use interface version
#  information CURRENT, REVISION, and AGE to build it (*note
#  Versioning::).  Do *not* use this flag to specify package release
#  information, rather see the `-release' flag
libflews_la_LDFLAGS=-version-info 0:0:0


nobase_include_HEADERS = FL/Fl_Ball.H\
                         FL/Fl_flews.h\
                         FL/Fl_Multi_Value_Slider.H\
                         FL/Fl_Pack_2D.H\
                         FL/Fl_Quaternion.H\
                         FL/Fl_Spin.H\
                         FL/Fl_Value_Input_Spin.H\
                         FL/Fl_Value_Slider_Input.H\
                         FL/gl_image.hpp


check_PROGRAMS      = testflews
testflews_SOURCES   = testflews.cpp
testflews_LDADD     = libflews.la @FLTK_LIBS@
testflews_CXXFLAGS  = @FLTK_CFLAGS@

SUFFIXES = .cpp .fl
.fl.cpp:
	fluid -c $<


pkgconfigdir = @pkgconfigdir@
pkgconfig_DATA = flews.pc
EXTRA_DIST = flews.pc.in

docdir = $(datarootdir)/doc/@PACKAGE@
doc_DATA = testflews.fl Makefile.example README
