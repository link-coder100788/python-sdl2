#!/bin/zsh

cd "$(dirname "$0")/../test"

leaks --atExit -- ruby test.rb