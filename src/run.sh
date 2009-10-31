#!/bin/bash

rm ./zemu &> /dev/null

if make ; then
	./zemu
else
	echo "[ There are some compilation errors ]"
fi
