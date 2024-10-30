#!/bin/bash
set -e

CONTRIB_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
ROOT_DIR="$(dirname "$CONTRIB_DIR")"
INSTALL_DIR="$ROOT_DIR/custom_install"

mkdir -p $INSTALL_DIR

CVC5_VERSION=cvc5-1.1.1

if [ "$(uname)" == "Darwin" ]; then
    NUM_CORES=$(sysctl -n hw.logicalcpu)
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    NUM_CORES=$(nproc)
else
    NUM_CORES=1
fi

$CONTRIB_DIR/setup-cadical.sh

if [ ! -d "$INSTALL_DIR/cvc5" ]; then
    cd $INSTALL_DIR
    git clone https://github.com/cvc5/cvc5.git
    cd cvc5
    git checkout ${CVC5_VERSION}
    ./configure.sh --static --auto-download --dep-path="$DEPS/install"
    cd build
    make -j$NUM_CORES
else
    echo "$INSTALL_DIR/cvc5 already exists. If you want to rebuild, please remove it manually."
fi

if [ -f $INSTALL_DIR/cvc5/build/src/libcvc5.a ] && [ -f $INSTALL_DIR/cvc5/build/src/parser/libcvc5parser.a ]; then
    echo "It appears cvc5 was setup successfully into $INSTALL_DIR/cvc5."
    echo "You may now install it with make ./configure.sh --cvc5 && cd build && make"
else
    echo "Building cvc5 failed."
    echo "You might be missing some dependencies."
    echo "Please see their github page for installation instructions: https://github.com/cvc5/cvc5"
    exit 1
fi
