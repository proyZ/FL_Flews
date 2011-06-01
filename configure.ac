AC_PREREQ([2.68])
AC_INIT([libflews],[0.3.1],
        [daniel.barrero@gmail.com],
        [flews],
        [http://danielbarrero.com/software/flews.tar.gz])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AC_LANG([C++])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
LT_INIT

AC_PROG_CXX

AC_CHECK_PROG(FLTK_CONFIG_FOUND,[fltk-config],[yes],[no])
if test "x${FLTK_CONFIG_FOUND}" != "xyes"; then
   AC_MSG_ERROR([package requirements (fltk) were not met: No package 'fltk' found]);
else
   FLTK_CFLAGS="$(fltk-config --cxxflags)"
   FLTK_LIBS="$(fltk-config --ldflags)"
fi


AC_CONFIG_FILES([ Makefile
                  flews.pc ])

AC_SUBST([pkgconfigdir],"${libdir}/pkgconfig")
AC_SUBST([FLTK_CFLAGS])
AC_SUBST([FLTK_LIBS])


AC_OUTPUT
