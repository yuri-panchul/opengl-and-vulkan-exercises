#!/usr/bin/env bash

set -eu

d=$(mktemp -d)

pwd="$PWD"
cd "$d"

glfw=~/opengl/glfw/glfw-3.3.8
glew=~/opengl/glew/installed

# cp "$pwd"/*.jpg "$pwd"/*.png .

if [ -d "$glfw" ] && [ -d "$glew" ]
then
    g++ -DGLEW_STATIC \
        -I "$pwd" -I $glfw/deps -I $glfw/include -I $glew/include -I "$pwd/../soil/build/include" \
        "$pwd/t.cpp" $glfw/deps/glad_gl.c \
        $glfw/src/libglfw3.a $glew/lib64/libGLEW.a "$pwd/../soil/build/lib/libSOIL.a" \
        -ldl -lGL -lpthread -lX11

elif [[ "$OSTYPE" = "darwin"* ]]; then
    clang++ "$pwd/t.cpp"  -std=c++11 -I/opt/homebrew/include -L/opt/homebrew/lib -framework OpenGL -lglew -lglfw
else
    gcc "$pwd/t.cpp" -lGL -lGLEW -lglfw
fi

./a.out
rm -rf "$d"
