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
 * @mainpage
 * EP Simulator is a simulation of a cardiac electrophysiology laboratory, 
 * complete with recording equipment, programmable stimulator, and,
 * most importantly, a heart simulator that can be set up to mimic
 * normal cardiac electrophysiology and arrhythmias.
 */

#include "navigator.h"

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#include <QtDebug>

// Languages
// Only define 1 of the below
//#define GERMAN
//#define FRENCH
#define ENGLISH

#ifndef ENGLISH
#   include <QtCore/QTranslator>
#endif

// Database
// Only define 1 of the below
#define MYSQL
//#define SQLITE

#ifdef MYSQL
#   define BACKEND_DB "QMYSQL"
#elif defined SQLITE
#   define BACKEND_DB "QSQLITE"
#endif

bool createConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase(BACKEND_DB);
    db.setHostName("localhost");
    db.setDatabaseName("epsimulator");
    db.setUserName("epsimuser");
    db.setPassword("epsimpassword");
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        return false;
    }
    return true;
}

int main(int argc, char **argv) {
    qDebug() << "Compiled using Qt Version " << qVersion();

    QApplication app(argc, argv);

    app.setOrganizationName("EP Studios");
    app.setOrganizationDomain("epstudiossoftware.com");
    app.setApplicationName("EPSimulator");
    app.setWindowIcon(QIcon(":/images/hi48-app-epsimulator.png"));
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
    if (!createConnection())
        return 1;
    EpNavigator::Navigator* navigator = new EpNavigator::Navigator;
    navigator->restore();
    return app.exec();
}
