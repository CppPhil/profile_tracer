#!/bin/bash

catch_errors() {
  printf "\nbuild.sh failed!\n" >&2
  exit 1
}

trap catch_errors ERR;

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

usage() {
  cat <<EOF
  build.sh [OPTIONS] Builds the project using the supplied options.

  EXAMPLE:
    build.sh --build_type=Debug --apply_patch=false

  OPTIONS:
  -h
    --help                this help text
  
  --build_type=BUILD_TYPE The build type to use (Debug | Release)

  --apply_patch=BOOLEAN   Whether to apply the Jaeger patch (true | false)
EOF
}

build_type="Debug"
apply_patch=false

while [ "$1" != "" ]; do
  PARAM=`echo $1 | awk -F= '{print $1}'`
  VALUE=`echo $1 | awk -F= '{print $2}'`
  case $PARAM in
    -h | --help)
      usage
      exit 0
      ;;
    --build_type)
      build_type=$VALUE
      ;;
    --apply_patch)
      apply_patch=$VALUE
      ;;
    *)
      echo "ERROR: unknown parameter \"$PARAM\""
      usage
      exit 1
      ;;
  esac
  shift
done

cd $DIR

./format.sh

if [ $apply_patch ]; then
  cd external/jaeger-client-cpp
  git apply $DIR/jaeger_patch.patch
fi

cd $DIR

if [ ! -d build ]; then
  mkdir build
fi

cd build

cmake -DCMAKE_BUILD_TYPE=$build_type -G "Unix Makefiles" ..
cmake --build . -- -j$(nproc)

if [ $apply_patch ]; then
  # Remove changes from the jaeger-client-cpp submodule
  cd $DIR/external/jaeger-client-cpp
  git checkout .
fi

cd $PREV_DIR

exit 0

