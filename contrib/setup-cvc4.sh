#!/bin/bash

#CVC4_VERSION=098c9f18375727af8b7a37bdf9f8658dcbc8cfd5
CVC4_VERSION=Thursday

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
DEPS=$DIR/../deps

mkdir -p $DEPS

if [ ! -d "$DEPS/CVC4" ]; then
    cd $DEPS
    git clone https://github.com/ying1123/CVC4.git
    chmod -R 777 CVC4
    cd CVC4
    git checkout -f $CVC4_VERSION
    ./contrib/get-antlr-3.4
    git clone https://github.com/uiri/toml.git
    export PYTHONPATH=$PYTHONPATH:`pwd`/toml
    CXXFLAGS=-fPIC CFLAGS=-fPIC ./configure.sh --static --no-static-binary debug
    cd build
    make -j16
    cd $DIR
else
    echo "$DEPS/CVC4 already exists. If you want to rebuild, please remove it manually."
fi

if [ -f $DEPS/CVC4/build/src/libcvc4.a ] && [ -f $DEPS/CVC4/build/src/parser/libcvc4parser.a ]; then
    echo "It appears CVC4 was setup successfully into $DEPS/CVC4."
    echo "You may now install it with make ./configure.sh --cvc4 && cd build && make"
else
    echo "Building CVC4 failed."
    echo "You might be missing some dependencies."
    echo "Please see their github page for installation instructions: https://github.com/CVC4/CVC4"
    exit 1
fi
