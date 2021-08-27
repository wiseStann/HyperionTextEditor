#!/bin/bash

sudo glib-compile-schemas /usr/share/glib-2.0/schemas/
sudo glib-compile-resources styles/ms.gresource.xml --target=src/resources.c --generate-source
