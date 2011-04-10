# Install EP Simulator

## Requirements

* [Qt SKD](http://qt.nokia.com/downloads) is recommended.
* Linux, Max OSX, or Windows

## Installation

Install Qt4 SDK.

Now unpack epsimulator, e.g.

    tar xfz epsimulator-vX.Y.Z.tar.gz
    cd epsimulator-vX.Y.Z/

It is easiest to run the programs using Qt Creator.  Add :`../lib` to the end
of the `LD_LIBRARY_PATH` variable under Build Environment on the Project page
of Qt Creator, otherwise the libraries won't link when you run the program.

The shared libraries are installed locally, so you can run the program and the
testing suite using these scripts in Linux or Max OS:

    bin/epsimulator.sh
    bin/testepsimulator.sh
    
Or in Windows:

    bin/epsimulator.exe
    bin/testepsimulator.exe

Finally (*nix only)

    bin/eraseall.sh

can be used to erase data and configuration files for testing.  In a final
version of the program, you would not want to erase your data!

