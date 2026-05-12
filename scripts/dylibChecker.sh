#!/bin/zsh

GREEN="\033[32m"
RESET="\033[0m"

echo -e "${GREEN}Python-SDL2 Product Dependency Checker${RESET}\n"
cd "$(dirname "$0")/../cmake-build-debug"
echo -e "\n${GREEN}Jar Classes:${RESET}\n"
jar tf JavaSDL2.jar
echo -e "\n${GREEN}Java Dylib Dependencies:${RESET}\n"
otool -L libjava_sdl2.dylib
echo -e "\n${GREEN}Python bundle Dependencies:${RESET}\n"
otool -L python_sdl2.cpython-314-darwin.so
echo -e "\n${GREEN}Reflection tool Dependencies:${RESET}\n"
otool -L reflect_tool
echo -e "\n${GREEN}Ruby bundle Dependencies:${RESET}\n"
otool -L ruby_sdl2.bundle