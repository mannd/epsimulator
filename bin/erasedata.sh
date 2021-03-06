#!/bin/bash
# This script erases all epsimulator catalog, settings and 
# data files!  Only works for my specific file locations.
# This is handy for test purposes, but don't run it
# on data you want to save!!
# Note location of configuration files is different in Qt4 vs 3

echo "Note that this script does not erase Windows data files"
dirname=`dirname $0`
if [ "${dirname:0:1}" != "/" ]; then
    dirname=$PWD/$dirname
fi
echo "Erasing from "$dirname/../System

rm -f ~/.config/EP\ Studios/EPSimulator.conf
rm -fr ~/MyStudies/*
rm -fr ~/epsim_studies
rm -f $dirname/../src/test/catalog.dat
rm -f $dirname/../System/amplifier.dat
rm -f $dirname/../System/catalog.dat
rm -f $dirname/../System/config.dat
rm -f $dirname/../System/columnformats.dat
rm -f $dirname/../System/eplists.dat
rm -f $dirname/../System/intervals.dat
rm -f $dirname/../System/label.dat
rm -f $dirname/../System/protocols.dat
rm -f $dirname/../System/windowsettings.dat
rm -fr $dirname/../System/studies

echo "Erasing from "$HOME/.epsimulator

rm -f $HOME/.epsimulator/amplifier.dat
rm -f $HOME/.epsimulator/catalog.dat
rm -f $HOME/.epsimulator/catalog.db
rm -f $HOME/.epsimulator/config.dat
rm -f $HOME/.epsimulator/columnformats.dat
rm -f $HOME/.epsimulator/eplists.dat
rm -f $HOME/.epsimulator/intervals.dat
rm -f $HOME/.epsimulator/label.dat
rm -f $HOME/.epsimulator/protocols.dat
rm -f $HOME/.epsimulator/windowsettings.dat
rm -fr $HOME/.epsimulator/studies
rm -fr $HOME/.epsimulator/cache
