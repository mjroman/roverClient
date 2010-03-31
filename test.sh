#!/bin/sh
if [ ! -e ./Makefile ]
then
	echo "Running Qmake"
	qmake 
fi

make && open build/QtRoverClient_debug.app
