AM_CPPFLAGS = $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS)
AM_LDFLAGS = $(SDL_LDFLAGS) $(SDL_image_LDFLAGS) $(SDL_mixer_LDFLAGS)
LDADD = $(SDL_LIBS) $(SDL_image_LIBS) $(SDL_mixer_LIBS)

## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = subdir-objects

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
## Autoreconf suggest the -I m4 flag.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

## The libtool archive file (.la) will be installed into the directory named
## by the predefined variable $(bindir), along with the actual shared library
## file (.so).
lib_LTLIBRARIES = libsdlc-@LIBSDLC_API_VERSION@.la

libsdlc_@LIBSDLC_API_VERSION@_la_SOURCES = SDLc/Surface.cpp \
					   SDLc/BaseSurface.cpp \
					   SDLc/Font.cpp \
					   SDLc/Fontdata.cpp \
					   SDLc/Input.cpp \
					   SDLc/Particle.cpp \
					   SDLc/Screen.cpp \
					   SDLc/Sprite.cpp \
					   SDLc/Timer.cpp \
					   SDLc/Mixer.cpp \
					   SDLc/Music.cpp \
					   SDLc/Sound.cpp

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libsdlc_@LIBSDLC_API_VERSION@_la_LDFLAGS = -version-info $(LIBSDLC_SO_VERSION)

## Define the list of public header files and their install location.  The
## nobase_ prefix instructs Automake to not strip the directory part from each
## filename, in order to avoid the need to define separate file lists for each
## installation directory.  This only works if the directory hierarchy in the
## source tree matches the hierarchy at the install location, however.
libsdlc_includedir = $(includedir)/libsdlc-$(LIBSDLC_API_VERSION)
nobase_libsdlc_include_HEADERS = SDLc.h \
				 SDLc/Surface.h \
				 SDLc/BaseSurface.h \
				 SDLc/Font.h \
				 SDLc/Input.h \
				 SDLc/Particle.h \
				 SDLc/Screen.h \
				 SDLc/Sprite.h \
				 SDLc/Timer.h \
				 SDLc/Mixer.h \
				 SDLc/Audio.h \
				 SDLc/Music.h \
				 SDLc/Sound.h

## The generated configuration header is installed in its own subdirectory of
## $(libdir).  The reason for this is that the configuration information put
## into this header file describes the target platform the installed library
## has been built for.  Thus the file must not be installed into a location
## intended for architecture-independent files, as defined by the Filesystem
## Hierarchy Standard (FHS).
## The nodist_ prefix instructs Automake to not generate rules for including
## the listed files in the distribution on 'make dist'.  Files that are listed
## in _HEADERS variables are normally included in the distribution, but the
## configuration header file is generated at configure time and should not be
## shipped with the source tarball.
libsdlc_libincludedir = $(libdir)/libsdlc-$(LIBSDLC_API_VERSION)/include
nodist_libsdlc_libinclude_HEADERS = libsdlcconfig.h

## Install the generated pkg-config file (.pc) into the expected location for
## architecture-dependent package configuration information.  Occasionally,
## pkg-config files are also used for architecture-independent data packages,
## in which case the correct install location would be $(datadir)/pkgconfig.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libsdlc-$(LIBSDLC_API_VERSION).pc
