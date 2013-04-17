m4_include(m4/ax_cxx_compile_stdcxx_11.m4)
m4_include(m4/acx_pthread.m4)

AC_PREREQ([2.68])
AC_INIT([libSDLc],
        [0.4.0],
        [jon@haeggblad.com],
        [libsdlc],
        [http://github.com/octol/libsdlc])
AC_CONFIG_SRCDIR([README])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h libsdlcconfig.h])
AM_INIT_AUTOMAKE
LT_INIT([disable-static])

AC_SUBST([LIBSDLC_SO_VERSION],[0:4:0])
AC_SUBST([LIBSDLC_API_VERSION],[0.4])

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
#AC_LANG([C++])
AC_LANG_PUSH([C++])
AX_CXX_COMPILE_STDCXX_11
AC_LANG_POP([C++])

#-------------------------------------------------------------------------------
# Add development flags.
#-------------------------------------------------------------------------------
#CXXFLAGS="-g -Og -std=gnu++11"
#CXXFLAGS="$CXXFLAGS -Wpedantic -Wall -W -Wextra"
#CXXFLAGS="$CXXFLAGS -Wcast-qual -Wwrite-strings"
#CXXFLAGS="$CXXFLAGS -Weffc++"
#CXXFLAGS="$CXXFLAGS -Wshadow"
#CXXFLAGS="$CXXFLAGS -Wconversion" 
#CXXFLAGS="$CXXFLAGS -fno-omit-frame-pointer"
#CXXFLAGS="$CXXFLAGS -fsanitize=address"
#CXXFLAGS="$CXXFLAGS -fno-inline"
#CXXFLAGS="$CXXFLAGS -fdiagnostics-color=auto"

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
AM_PATH_SDL($SDL_VERSION,:,
	    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CFLAGS="$CFLAGS $SDL_CFLAGS"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

#-------------------------------------------------------------------------------
# Check for SDL_mixer
#-------------------------------------------------------------------------------
have_SDLmixer=no
AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio, have_SDLmixer=yes)
if test x$have_SDLmixer != xyes; then
    AC_MSG_ERROR([*** Can not find the SDL_mixer library
Try: http://www.libsdl.org/projects/SDL_mixer/
])
fi
LIBS="$LIBS -lSDL_mixer"

#-------------------------------------------------------------------------------
# Check for SDL_image
#-------------------------------------------------------------------------------
have_SDLimage=no
AC_CHECK_LIB(SDL_image, IMG_Load_RW, have_SDLimage=yes)
if test x$have_SDLimage != xyes; then
    AC_MSG_ERROR([*** Can not find the SDL_image library
Try: http://www.libsdl.org/projects/SDL_image/
])
fi
LIBS="$LIBS -lSDL_image"

# Override the template file name of the generated .pc file, so that there
# is no need to rename the template file when the API version changes.
AC_CONFIG_FILES([Makefile
                 libsdlc-${LIBSDLC_API_VERSION}.pc:libsdlc.pc.in])
AC_OUTPUT
