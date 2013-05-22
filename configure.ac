m4_include(m4/ax_cxx_compile_stdcxx_11.m4)
m4_include(m4/acx_pthread.m4)

AC_PREREQ([2.69])
AC_INIT([libSDLc],[0.5.3],[jon@haeggblad.com],[libsdlc],[http://www.haeggblad.com/libsdlc])
AC_CONFIG_SRCDIR([README])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h libsdlcconfig.h])
AM_INIT_AUTOMAKE
LT_INIT([disable-static])

AC_SUBST([LIBSDLC_SO_VERSION],[4:0:1])
AC_SUBST([LIBSDLC_API_VERSION],[0.5.3])

DEBUG_CXXFLAGS="-g -Og -std=gnu++11 \
-Wall -W -Wextra -Wpedantic -Wcast-qual -Wwrite-strings -Wconversion \
-fno-omit-frame-pointer -fno-inline -fdiagnostics-color=auto"
#DEBUG_CXXFLAGS="$DEBUG_CXXFLAGS -Weffc++"

#-------------------------------------------------------------------------------
# Checks for programs.
#-------------------------------------------------------------------------------
AC_PROG_CC
AC_PROG_CXX
AC_CHECK_FUNCS([atexit])
AC_C_INLINE
AC_HEADER_STDBOOL
AC_PROG_INSTALL
AC_TYPE_SIZE_T

#-------------------------------------------------------------------------------
# Check for C++11.
#-------------------------------------------------------------------------------
AC_LANG_PUSH([C++])
AX_CXX_COMPILE_STDCXX_11
AC_LANG_POP([C++])

#-------------------------------------------------------------------------------
# Configure pthreads.
#-------------------------------------------------------------------------------
AC_ARG_WITH([pthreads],
            [AS_HELP_STRING([--with-pthreads],
               [use pthreads (default is yes)])],
            [with_pthreads=$withval],
            [with_pthreads=check])

have_pthreads=no
AS_IF([test "x$with_pthreads" != "xno"],
      [ACX_PTHREAD(
        [],
        [AS_IF([test "x$with_pthreads" != "xcheck"],
               [AC_MSG_FAILURE(
                 [--with-pthreads was specified, but unable to be used])])])
       have_pthreads="$acx_pthread_ok"])
AM_CONDITIONAL([HAVE_PTHREADS],[test "x$have_pthreads" == "xyes"])
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_LIBS)

#-------------------------------------------------------------------------------
# Check for SDL
#-------------------------------------------------------------------------------
SDL_VERSION=1.2.0
PKG_CHECK_MODULES(SDL,sdl >= $SDL_VERSION, [], 
                  AC_MSG_ERROR([SDL library >= $SDL_VERSION not found!]))

PKG_CHECK_MODULES(SDL_mixer, SDL_mixer >= $SDL_VERSION ,[],
	          AC_MSG_ERROR([SDL_mixer library >= $SDL_VERSION not found]))

PKG_CHECK_MODULES(SDL_image, SDL_image >= $SDL_VERSION ,[],
	          AC_MSG_ERROR([SDL_image library >= $SDL_VERSION not found]))

#-------------------------------------------------------------------------------
# Debug build switch
# from http://www.bioinf.uni-freiburg.de/~mmann/HowTo/automake.html#debug
#-------------------------------------------------------------------------------
AC_MSG_CHECKING([whether to enable the debug build])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug build (def=no)])],
    [enable_debug="$enableval"],
    [enable_debug=no])
AC_MSG_RESULT([$enable_debug])

if test x"$enable_debug" = x"yes"; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
    CXXFLAGS="$DEBUG_CXXFLAGS"
else
    AC_DEFINE([NDEBUG],[],[Release Mode])
fi

#-------------------------------------------------------------------------------
# Output result
#-------------------------------------------------------------------------------

# Override the template file name of the generated .pc file, so that there
# is no need to rename the template file when the API version changes.
AC_CONFIG_FILES([Makefile
                 libsdlc-${LIBSDLC_API_VERSION}.pc:libsdlc.pc.in])
AC_OUTPUT
AC_MSG_RESULT([
    SDLc $VERSION is now configured

    Source directory: $srcdir
    Installation prefix: $prefix
    C++ compiler: $CXX $CXXFLAGS
    Debug build: $enable_debug
])
