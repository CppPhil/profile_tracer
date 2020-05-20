#!/bin/bash

catch_errors() {
  printf "\nrun.sh failed!\n" >&2
  exit 1
}

trap catch_errors ERR;

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

cd $DIR

./build/profile_tracer_app

cd $PREV_DIR

exit 0

