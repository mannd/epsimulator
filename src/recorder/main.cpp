/***************************************************************************
 *   Copyright (C) 2006 by EP Studios, Inc.                                *
 *   mannd@epstudiossoftware.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/** 
    \file main.cpp
    Main program file for Recorder module of EP Simulator.
*/

/**
    \mainpage
    EP Simulator is a simulation of a cardiac electrophysiology laboratory, 
    complete with recording equipment, programmable stimulator, and, most importantly, 
    a heart simulator that can be set up to mimic 
    normal cardiac electrophysiology and arrhythmias.
*/

// Note that below is Froglogics GetOpt class, but header file is
// renamed from getopt.h to getopts.h because of conflict with 
// unix getopt.h.
#include "getopts.h"
#include "options.h"
#include "recorder.h"
#include "versioninfo.h"

#include <qapplication.h>
#include <qmessagebox.h>
#include <qstring.h>

// Languages
// Only define 1 of the below
//#define GERMAN
//#define FRENCH
#define ENGLISH

#ifndef ENGLISH
#include <qtranslator.h>
#endif

#include <exception>
#include <iostream>

using std::endl;
using std::cerr;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

// International stuff below
#ifndef ENGLISH
    QTranslator translator( 0 );
#ifdef GERMAN
    translator.load( "epsimulator_de.qm", "." );
#endif
#ifdef FRENCH
    translator.load( "epsimulator_fr.qm", "." );
#endif
    app.installTranslator( &translator );
#endif

    VersionInfo* versionInfo = VersionInfo::instance();
    GetOpt opts;
//     QString path;
//     opts.addOption('p', "path", &path);
//     bool help;
//     opts.addSwitch("help", &help);
//     if (!opts.parse()) {
//         cerr << QObject::tr("Usage:  %1 [--path=filename]").arg(versionInfo->appName()) 
//             << endl << endl;
//         return 1;
//     }
//     if (help) {
//         cerr << 
//             QObject::tr("%1 version %2").arg(versionInfo->programName())
//                      .arg(versionInfo->version())
//             << endl << QObject::tr("Copyright (c) %1 EP Studios, Inc.")
//                .arg(versionInfo->copyrightYear())
//             << endl << QObject::tr("See www.epstudiossoftware.com for help.")
//             << endl << endl; 
//         return 1;
//     }
    Options* options = Options::instance();
//     if (! path.isEmpty()) 
//         options->setTempStudyPath(path);
    Recorder *mainWin = new Recorder();
    app.setMainWidget(mainWin);
    // Below is a work-around as showMaximized() alone doesn't always work.
    mainWin->showNormal();
    mainWin->showMaximized();
    return app.exec();
    // delete the singletons -- only need to do here at end of program!
    options->destroy();
    versionInfo->destroy();
}
