#!/bin/sh

FLAGS="--enable-static --disable-shared"
CONFIG_SHELL=/bin/sh
export CONFIG_SHELL
#PREFIX=/usr/local/cross-tools
PREFIX=/home/jon/src/mxe/usr
if [ "$TARGET" = "" ]; then
    TARGET=i686-pc-mingw32
fi
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
cache=cross-config.cache
if [ -f configure ]; then
    CONFIGURE=configure
elif [ -f ../configure ]; then
    CONFIGURE=../configure
else
    echo "Couldn't find configure - aborting!"
    exit 2
fi
sh $CONFIGURE --cache-file="$cache" --host=$TARGET --prefix=$PREFIX/$TARGET $FLAGS $*
status=$?
rm -f "$cache"
exit $status
