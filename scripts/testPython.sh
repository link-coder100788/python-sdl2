#!/bin/zsh

cd "$(dirname "$0")/../test"

PYTHONPATH=../cmake-build-debug/ python3 test.py