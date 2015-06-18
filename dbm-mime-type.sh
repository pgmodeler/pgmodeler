#!/bin/bash

# This script is used by the installer to 
# update the .dbm file association by using pgmodeler-cli.

# Loading needed environment variables
WORKING_DIR="$(dirname "$(readlink -f "$0")")"
source "$WORKING_DIR/pgmodeler.vars"
$PGMODELER_CLI_PATH "--dbm-mime-type" "$@"
