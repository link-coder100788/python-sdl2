#!/bin/zsh

cd "$(dirname "$0")/../test"

javac -cp ../cmake-build-debug/JavaSDL2.jar Test.java
java -XstartOnFirstThread --enable-native-access=ALL-UNNAMED -Djava.library.path=../cmake-build-debug/ -cp ../cmake-build-debug/JavaSDL2.jar:. Test
