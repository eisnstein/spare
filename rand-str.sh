#!/bin/bash

DEFAULT_LENGTH=64
LENGTH=${1:-$DEFAULT_LENGTH}

tr -dc A-Za-z0-9 </dev/urandom | head -c $LENGTH > file1.txt