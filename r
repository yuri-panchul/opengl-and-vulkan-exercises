#!/usr/bin/env bash

set -eu

d=$(mktemp -d)

pwd="$PWD"
cd "$d"

g++ "$pwd/t.cpp" -lGLEW -lglfw

./a.out
rm -rf "$d"
