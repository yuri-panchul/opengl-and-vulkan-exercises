#!/usr/bin/env bash

set -eu

d=$(mktemp -d)

pwd="$PWD"
cd "$d"

gcc "$pwd/t.cpp" -lGL -lGLEW -lglfw

./a.out
rm -rf "$d"
