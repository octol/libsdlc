#!/bin/sh

#PREFIX=/usr/local/cross-tools
PREFIX=/home/jon/src/mxe/usr
if [ "$TARGET" = "" ]; then
    TARGET=i686-pc-mingw32
fi
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
exec make $*
