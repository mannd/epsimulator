#!/bin/bash
# This script erases all epsimulator catalog, settings and 
# data files!  Only works for my specific file locations.
# This is handy for test purposes, but don't run it
# on data you want to save!!
# Note location of configuration files is different in Qt4 vs 3

echo "Note that this script does not erase Windows data files"
echo "Erasing "$HOME/.epsimulator/epsimulator.db

rm -f $HOME/.epsimulator/epsimulator.db
