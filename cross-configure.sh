#!/bin/sh

CONFIG_SHELL=/bin/sh
export CONFIG_SHELL
#PREFIX=/usr/local/cross-tools
PREFIX=/home/jon/src/mxe/usr
if [ "$TARGET" = "" ]; then
    TARGET=i686-pc-mingw32
fi
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
#if [ -f "$PREFIX/$TARGET/bin/$TARGET-sdl-config" ]; then
if [ -f "$PREFIX/bin/$TARGET-sdl-config" ]; then
    #SDL_CONFIG="$PREFIX/$TARGET/bin/$TARGET-sdl-config"
    SDL_CONFIG="$PREFIX/bin/$TARGET-sdl-config"
    export SDL_CONFIG
    echo $SDL_CONFIG
fi
cache=cross-config.cache
if [ -f configure ]; then
    CONFIGURE=configure
elif [ -f ../configure ]; then
    CONFIGURE=../configure
else
    echo "Couldn't find configure - aborting!"
    exit 2
fi
sh $CONFIGURE --cache-file="$cache" \
	--target=$TARGET --host=$TARGET --build=i386-linux \
	--prefix=$PREFIX/$TARGET --enable-static --disable-shared $*
status=$?
rm -f "$cache"
exit $status
