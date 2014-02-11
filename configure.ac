dnl require autoconf 2.60 (AS_ECHO/AS_ECHO_N)
AC_PREREQ([2.60])
define(_CLIENT_VERSION_MAJOR, 0)
define(_CLIENT_VERSION_MINOR, 1)
define(_CLIENT_VERSION_IS_RELEASE, false)
define(_COPYRIGHT_YEAR, 2014)
AC_INIT([cj-client],[_CLIENT_VERSION_MAJOR._CLIENT_VERSION_MINOR],[apoelstra@wpsoftware.net],[cj-client])
AC_CONFIG_MACRO_DIR([m4])

dnl ==============================================================
dnl Setup for automake
dnl ==============================================================

AM_INIT_AUTOMAKE([-Wall foreign])

dnl faketime messes with timestamps and causes configure to be re-run.
dnl --disable-maintainer-mode can be used to bypass this.
AM_MAINTAINER_MODE([enable])

AC_CONFIG_SRCDIR([./joiner.c])
AC_CONFIG_HEADERS([./config.h])

dnl Checks for programs.
AM_PROG_AR
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_SED
AM_CONDITIONAL([GCC], [test x$GCC = xyes])
AC_PATH_PROG([GIT], [git])
PKG_PROG_PKG_CONFIG

dnl cflags
CFLAGS="$CFLAGS -W -Wall -Wextra -std=c99 -pedantic -g"
LDFLAGS="$LDFLAGS -lm"

dnl Special case build systems
use_pkgconfig=yes
case $host in
  *mingw*)
    dnl pkgconfig does more harm than good with MinGW
    use_pkgconfig=no

    TARGET_OS=windows

    AX_CHECK_LINK_FLAG([[-static-libgcc]],[LDFLAGS="$LDFLAGS -static-libgcc"])

    CFLAGS="$CFLAGS -DWIN32 -D__WINDOWS__ -D__USE_MINGW_ANSI_STDIO"
esac

AC_CHECK_HEADERS([assert.h ctype.h stdio.h stdlib.h string.h])

PKG_CHECK_MODULES([CJ_CLIENT], [
  glib-2.0
  gobject-2.0
  gtk+-3.0
  libcurl
])

AC_CONFIG_FILES([
    Makefile
    jansson/jansson.pc
    jansson/Makefile
    jansson/src/Makefile
    jansson/doc/Makefile
    jansson/test/Makefile
    jansson/src/jansson_config.h
    jansson/test/bin/Makefile
    jansson/test/suites/Makefile
    jansson/test/suites/api/Makefile
])

### lifted directly from jansson/configure.ac
# Checks for header files.
AC_CHECK_HEADERS([locale.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT32_T
AC_TYPE_LONG_LONG_INT

AC_C_INLINE
case $ac_cv_c_inline in
    yes) json_inline=inline;;
    no) json_inline=;;
    *) json_inline=$ac_cv_c_inline;;
esac
AC_SUBST([json_inline])

# Checks for library functions.
AC_CHECK_FUNCS([strtoll localeconv])

case "$ac_cv_type_long_long_int$ac_cv_func_strtoll" in
     yesyes) json_have_long_long=1;;
     *) json_have_long_long=0;;
esac
AC_SUBST([json_have_long_long])

case "$ac_cv_header_locale_h$ac_cv_func_localeconv" in
     yesyes) json_have_localeconv=1;;
     *) json_have_localeconv=0;;
esac
AC_SUBST([json_have_localeconv])
### END lifted directly from jansson/configure.ac

AC_OUTPUT

