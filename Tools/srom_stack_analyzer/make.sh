#!/bin/bash
mkdir -p build
cc -Iinclude main.c platform.c stack.c utils.c -o build/srom_stack_analyzer
