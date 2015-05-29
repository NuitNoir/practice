#!/bin/bash

gcv() {
g++ -Wall -Wextra $@ $(pkg-config --cflags --libs opencv)
}
gcv cg.cpp -o cg
./cg


