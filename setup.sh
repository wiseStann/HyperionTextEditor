#!/bin/bash

SCHEMA_FILE="hyper.gschema.xml"
SCHEMAS_DIR_PATH="/usr/share/glib-2.0/schemas"

GRESOURCE_FILE="hyper.gresource.xml"

if [ ! -f $SCHEMAS_DIR_PATH/$FILE ]; then
    sudo cp $SCHEMA_FILE $SCHEMAS_DIR_PATH/$SCHEMA_FILE
fi

sudo glib-compile-schemas $SCHEMAS_DIR_PATH
sudo glib-compile-resources $GRESOURCE_FILE --target=src/resources.c --generate-source
