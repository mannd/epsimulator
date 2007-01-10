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
 *  \file main.cpp
 *  Main program file for EP Simulator.
 */

/**
 *  \mainpage
 *  EP Simulator simulates an electrophysiology laboratory.  In particular it simulates
 *  the recording system, the stimulator, catheter placement, and arrhythmias.
 */

#include "epsimdefs.h"
// Note that below is Froglogics GetOpt class, but header file is
// renamed from getopt.h to getopts.h because of conflict with 
// unix getopt.h.
#include "getopts.h"
#include "options.h"
#include "navigator.h"

#include <qapplication.h>
#include <qstring.h>

#ifndef ENGLISH
#include <qtranslator.h>
#endif

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

    GetOpt opts;
    QString path;
    opts.addOption('p', "path", &path);
    bool help;
    opts.addSwitch("help", &help);
    if (!opts.parse()) {
        cerr << QObject::tr("Usage:  %1 [--path=filename]").arg(APP_NAME) 
            << endl << endl;
        return 1;
    }
    if (help) {
        cerr << 
            QObject::tr("%1 version %2").arg(PROGRAM_NAME).arg(VERSION)
            << endl << QObject::tr("Copyright (c) 2006 EP Studios, Inc.")
            << endl << QObject::tr("See www.epstudiossoftware.com for help.")
            << endl << endl; 
        return 1;
    }
    Options* options = Options::instance();
    if (! path.isEmpty()) 
        options->setTempStudyPath(path);
    Navigator *mainWin = new Navigator();
    app.setMainWidget(mainWin);
    mainWin->showMaximized();
    return app.exec();
    // deletes the options instance -- can only be used at end of program!
    options->destroy();
}
