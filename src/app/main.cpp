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

#include "coreconstants.h"
#include "fileutilities.h"
#include "localstorage.h"
#include "navigator.h"
#include "networkstorage.h"
#include "options.h"
#include "systemstorage.h"

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#include <QtDebug>

// Languages -- only define 1 of the below
//#define GERMAN
//#define FRENCH
#define ENGLISH

#ifndef ENGLISH
#   include <QtCore/QTranslator>
#endif

static EpCore::Options* options = 0;

static bool loadOptions() {
    options = EpCore::Options::instance();
    if (options)
        options->load();
    else
        QMessageBox::critical(0, QObject::tr("Options Load Error"),
                              QObject::tr("Cannot load System options."));
    return options;
}

static void deleteOptions() {
    // any change in options should already have been saved by now
    delete options;
}

static void disableNetworkStorage() {
    QMessageBox::information(0, QObject::tr("Network Storage Path Not Valid"),
                             QObject::tr("Network path is not a valid path. "
                                         "Please use System Settings dialog "
                                         "under Tools menu to enter a valid "
                                         "Network path."));
    EpCore::clearFlag(options->filePathFlags, 
                      EpCore::Options::EnableNetworkStorage);
    options->save();
}


static bool createSystemStorage() {
    EpCore::SystemStorage systemStorage;
    // SystemStorage should never change during program
    // so you should only have to init() it once here.
    if (!systemStorage.init()) {   
        QMessageBox::critical(0, QObject::tr("System Storage Error"),
                              QObject::tr("Cannot find or create "
                                          "default System storage path %1.")
                              .arg(systemStorage.path()));
        return false;
    }
    return true;
}

static bool createNetworkStorage() {
    using EpCore::Options;
    if (options->filePathFlags.testFlag(Options::EnableNetworkStorage)) {
        EpCore::NetworkStorage networkStorage(options->networkStudyPath);
        if (!networkStorage.init()) {
            QMessageBox::information(0, QObject::tr("Network Storage Error"),
                                     QObject::tr("Cannot find or create "
                                                 "Network storage path %1. "
                                                 "Use System Settings dialog "
                                                 "to set up Network.")
                                     .arg(networkStorage.path()));
            return false;
        }
    }
    return true;
}

static bool copyDefaultDatabase(const QString& path) {
	// copy language specific default database
#ifdef ENGLISH
	QString langSubDir = "en";
#elif defined GERMAN
	QString langSubDir = "de";
#elif defined FRENCH
	QString langSubDir = "fr";
#else
        QString langSubDir = "None";
#endif
        Q_ASSERT(langSubDir != "None");
        QString defaultDbPath = 
            EpCore::joinPaths(EpCore::rootPath(),
                              "db", langSubDir,
                              EpCore::Constants::EPSIM_DB_FILENAME);
	return QFile::copy(defaultDbPath, path);
}

static bool createLocalStorage() {
    EpCore::LocalStorage localStorage;
    // default hard drive studies directory is created if not already present
    if (!localStorage.init()) {
        QMessageBox::critical(0, QObject::tr("Local Storage Error"),
                              QObject::tr("Cannot find or create "
                                          "default Local storage path %1.")
                              .arg(localStorage.hardDrivePath()));
        return false;
    }
    return true;
}

// The default database is set to either the Network or System path.
// This is the database that stores lists, etc. -- not the study database.
static bool createConnection() {
    EpCore::SystemStorage systemStorage;
    using EpCore::Options;
    using EpCore::joinPaths;
    const QString dbFileName(EpCore::Constants::EPSIM_DB_FILENAME);
    QString systemDbFilePath = systemStorage.filePath(dbFileName);
    // always create a System database
    if (!QFile::exists(systemDbFilePath) && 
        !copyDefaultDatabase(systemDbFilePath)) {
        QMessageBox::critical(0, QObject::tr("System Database Error"),
                              QObject::tr("Cannot find or create "
                                          "default System Database file %1.")
                              .arg(systemDbFilePath));
        return false;
    }
    QString dbFilePath(systemStorage.filePath(dbFileName));
    if (options->includeNetworkCatalog()) {
        EpCore::NetworkStorage networkStorage(options->networkStudyPath);
        Q_ASSERT(networkStorage.exists()); // must exist or was disabled
                                           // prior to this!
    }

    //         QString networkDbFilePath = networkStorage.filePath(dbFileName);
    //         if (!copyDefaultDatabase(networkDbFilePath)) {
    //         QMessageBox::critical(0, QObject::tr("Network Database Error"),
    //     			  QObject::tr("Cannot find or create "
    //     				      "default Database file."));
    //         return false;
    //     }

    // }
    // // If no network database found, 
    // if (!QFile::exists(dbFilePath)) {
    //     if (options->includeNetworkCatalog()) {
    //         QMessageBox::question(0, QObject::tr("No Network Database Found"),
    //                               QObject::tr("Do you want to copy present "
    //                                           "System database to the network, "
    //                                           "or start with a fresh "
    //                                           "network database?"));
    //         // process copy database here

    //     }
    //     // copy language specific default database
    //     if (!copyDefaultDatabase(dbFilePath)) {
    //         QMessageBox::critical(0, QObject::tr("Database Error"),
    //     			  QObject::tr("Cannot find or create "
    //     				      "default Network "
    //                                           "Database file %1.")
    //                               .arg(networkDbFilePath)));
    //         return false;
    //     }
    // }
    QSqlDatabase db = 
        QSqlDatabase::addDatabase(EpCore::Constants::EPSIM_BACKEND_DB);
    db.setHostName(EpCore::Constants::EPSIM_DB_HOSTNAME);
    db.setDatabaseName(dbFilePath);
    db.setUserName(EpCore::Constants::EPSIM_DB_USERNAME);
    db.setPassword(EpCore::Constants::EPSIM_DB_PASSWORD);
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        return false;
    }
    return true;
}

static QString systemCatalogDbFilePath() {
    EpCore::SystemStorage systemStorage;
    Q_ASSERT(systemStorage.exists());
    const QString catalogDbFileName(EpCore::Constants::
                                    EPSIM_CATALOG_DB_FILENAME);
    return systemStorage.filePath(catalogDbFileName);
}

static bool createEmptyCatalog() {
    if (!QFile::exists(systemCatalogDbFilePath())) {
        if (!QFile::copy(EpCore::joinPaths(EpCore::rootPath(),
                                           "db/",
                                           QString(EpCore::Constants::
                                                   EPSIM_CATALOG_DB_FILENAME)), 
                         systemCatalogDbFilePath())) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  QObject::tr("Cannot find or create "
                                              "system catalog database."));
            return false;
        }
    }
    return true;
}

static bool createSystemCatalogDbConnection() {
    QSqlDatabase db = 
        QSqlDatabase::addDatabase(EpCore::Constants::EPSIM_BACKEND_DB,
                                  EpCore::Constants::EPSIM_SYSTEM_DB);
    db.setHostName(EpCore::Constants::EPSIM_DB_HOSTNAME);
    db.setDatabaseName(systemCatalogDbFilePath());
    db.setUserName(EpCore::Constants::EPSIM_DB_USERNAME);
    db.setPassword(EpCore::Constants::EPSIM_DB_PASSWORD);
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        return false;
    }
    return true;
}

static void displayVersion() {
    qDebug() << "EP Simulator"
             << "Version"
             << EpCore::Constants::EPSIM_VERSION
             << "Build"
             << EpCore::Constants::APP_VERSION_BUILD_STR;
    qDebug() << "Compiled using Qt Version"
             << QT_VERSION_STR;
    qDebug() << "Running on Qt Version"
             << qVersion();
}

// These are notifications of constraints used for compilation.
static void displayMessages() {
    using EpCore::Options;
    if (!options->opticalDiskFlags.testFlag(Options::AllowRealOpticalDisk))
        qDebug() << "No removable optical disk use permitted.";
    if (!options->opticalDiskFlags.testFlag(Options::AllowEmulation))
        qDebug() << "No optical disk emulation permitted.";
}

static void displayHelp() {
    const char* const helpText =
        "Usage:\n"
        "epsimulator [OPTION]\n"
        "Options:\n"
        "    --help          Display this help\n"
        "    --version       Display program version\n"
        "    --testcd        Display CD drive status";
    qWarning("%s", helpText);
}

static void displayError(const char* const option) {
    qWarning("epsimulator: unknown option %s", option);
}

static void testCd() {
    EpCore::testCdTools();
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QStringList arguments = app.arguments();
    if (arguments.contains("--help") || arguments.contains("-h")) {
        displayHelp();
        return 0;
    }
    if (arguments.contains("--version") || arguments.contains("-v")) {
        displayVersion();
        return 0;	// quit after displaying version info
    }
    if (arguments.contains("--testcd")) {
        testCd();
        return 0;
    }
    if (argc > 1) {     // hey, some other unknown option was given
        displayError(argv[1]);
        return 1;
    }
    // display version information with routine start
    displayVersion();

    app.setOrganizationName("EP Studios");
    app.setOrganizationDomain("epstudiossoftware.com");
    app.setApplicationName("EPSimulator");
    app.setWindowIcon(QIcon(":/images/hi48-app-epsimulator.png"));

    // International stuff below
#ifndef ENGLISH
    QTranslator translator(0);
#ifdef GERMAN
    translator.load("epsimulator_de.qm", ".");
#endif
#ifdef FRENCH
    translator.load("epsimulator_fr.qm", ".");
#endif
    app.installTranslator(&translator);
#endif

    if (!createSystemStorage())
        return 1;
    if (!createLocalStorage())
        return 1;
    // SystemStorage and LocalStorage must be created before database
    // connection or Options used.
    if (!loadOptions())
        return 1;
    if (!createNetworkStorage())
        disableNetworkStorage();
    if (!createConnection())
        return 1;
    if (!createEmptyCatalog())
        return 1;
    if (!createSystemCatalogDbConnection())
        return 1;
    displayMessages();          // special messages/settings are displayed
    deleteOptions();

    EpNavigator::Navigator* navigator = new EpNavigator::Navigator;
    navigator->restore();
    return app.exec();
}
