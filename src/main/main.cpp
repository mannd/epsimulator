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
 * @file main.cpp
 * Main program file for EP Simulator.
 */

/**
 * @mainpage
 * EP Simulator is a simulation of a cardiac electrophysiology laboratory, 
 *  complete with recording equipment, programmable stimulator, and, most importantly, 
 *  a heart simulator that can be set up to mimic 
 *   normal cardiac electrophysiology and arrhythmias.
 */

//#include "options.h"
#include "navigator.h"
#include "versioninfo.h"

#include <QApplication>
#include <QIcon>
#include <QMessageBox>
#include <QString>

// Languages
// Only define 1 of the below
//#define GERMAN
//#define FRENCH
#define ENGLISH

#ifndef ENGLISH
#include <QTranslator>
#endif

using EpNavigator::Navigator;
using EpCore::VersionInfo;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    app.setOrganizationName("EPStudios");
    app.setOrganizationDomain("epstudiossoftware.com");
    app.setApplicationName("EPSimulator");

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
    app.setWindowIcon(QIcon(":/images/hi48-app-epsimulator.png"));
    Navigator* mainWin = new Navigator;
    mainWin->show();
    return app.exec();
    // delete the singletons -- only need to do here at end of program!
    versionInfo->destroy();
}
