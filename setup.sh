#!/bin/bash

sudo mv hyper.gschema.xml /usr/share/glib-2.0/schemas
sudo glib-compile-schemas /usr/share/glib-2.0/schemas/
sudo glib-compile-resources styles/hyper.gresource.xml --target=src/resources.c --generate-source
