#!/bin/bash

cd "$(dirname "$0")"

mkdir -p ./analyze
cd ./analyze

pvs-studio-analyzer analyze -o analyze.log -i -j 2 -f ../build/compile_commands.json \
&& plog-converter -a GA:1,2 -t tasklist -o analyze.tasks analyze.log
