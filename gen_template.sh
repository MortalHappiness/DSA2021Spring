#!/bin/bash
set -e

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 [problem_folder] [problem_id]"
    exit 1
fi

problem_folder=$1
problem_id=$2
mkdir -p "$problem_folder"
cp template/main.c "$problem_folder/main.c"
sed "s/problem_id/$problem_id/g" template/Makefile > "$problem_folder/Makefile"
