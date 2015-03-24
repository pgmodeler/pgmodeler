#!/bin/bash

# Use this script if you having problems running pgModeler.
# The most common execution problem is the missing library error:
# [ ./pgmodeler: error while loading shared libraries: libutils.so.1: cannot open shared object file: No such file or directory ]

# This script configures all the needed enviroment variables but does not affects the configuration of the whole system.
# The only downside of this script is that you can not call the "pgmodeler" executable directly, being obligated
# to use this script every time you want to run the application.

# For a more elaborated solution you may find useful the usage of pgmodeler.vars file.
# Details about the use of that file are in it's comments.

# Loading needed environment variables
source "./pgmodeler.vars"

"$PGMODELER_ROOT/pgmodeler" "$@" &
