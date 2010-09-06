# EP Simulator

## Purpose

**EP Simulator** is intended to simulate the operation of the GE/Prucka Cardiac
Electrophysiology recording system.  

## Note to Developers

**EP Simulator** can be built and run using Qt Creator.  Just open the
`epsimulator.pro` file inside Qt Creator.

Add :`../lib` to the end of the `LD_LIBRARY_PATH` variable under Build
Environment on the Project page of Qt Creator, otherwise the libraries
won't link when you run the program.

If you want to run or compile the program outside of the Qt Creator
environment, use the `epsimulator.sh` shell program in the `bin` directory
to launch the program in Linux, or double-click the `epsimulator.exe`
program in the bin directory in Windows.

## Recent Notes

At present I am developing on a local branch without much updating to GitHub.
I'm working on adding a backend database (sqlite) for storage of master lists
and patient catalog information, instead of the present binary files.  This is
turning into a significant rewrite.
