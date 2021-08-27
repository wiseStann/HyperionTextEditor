#!/bin/bash

DEST_DIR="bin"

if [ ! -f DEST_DIR ]; then
    meson setup $DEST_DIR
fi

if [ -d $DEST_DIR ]; then
    ninja -C $DEST_DIR
fi