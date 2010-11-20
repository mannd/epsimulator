/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

#include "abstractmainwindow.h"

#include "actions.h"
#include "changepassworddialog.h"
#include "coreconstants.h"
#include "editintervalsdialog.h"
#include "editcolumnformatsdialog.h"
#include "editprotocolsdialog.h"
#include "error.h"
#include "fileutilities.h"
#include "opticaldisk.h"
#include "options.h"
#include "passworddialog.h"
#include "simulatorsettingsdialog.h"
#include "systemdialog.h"
#include "user.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QUrl>

using EpCore::Options;
using EpCore::User;
using EpGui::AbstractMainWindow;
using EpHardware::EpOpticalDisk::OpticalDisk;
using namespace EpCore::Constants;

AbstractMainWindow::AbstractMainWindow(Options* options, 
                                       User* user,
                                       QWidget *parent)
    : QMainWindow(parent), options_(options), user_(user) {
    createActions();
}

AbstractMainWindow::~AbstractMainWindow() {}

void AbstractMainWindow::simulatorSettings() {
    if (administrationAllowed()) {
        SimulatorSettingsDialog simDialog(options_, this);
        if (simDialog.exec() == QDialog::Accepted) {
            simDialog.setOptions();
            updateSimulatorSettings();
            options_->save();
        }
    }
}

void AbstractMainWindow::changeDatabase() {
    if (options_->includeNetworkCatalog()) {
        QString networkPath = options_->networkStudyPath;
        QString networkDbFilePath = 
            EpCore::joinPaths(networkPath, EPSIM_DB_FILENAME);
        if (!QFile::exists(networkDbFilePath)) {
            QMessageBox::information(0, QObject::tr("Database Error"),
				     QObject::tr("Cannot find Network "
						 "Database file. "
						 "Will use local "
						 "Database file. "
						 "Network storage is "
						 "disabled."));
	    // get rid of network storage until user fixes the problem
	    EpCore::clearFlag(options_->filePathFlags, 
                              Options::EnableNetworkStorage);
            return;
        }
        // Note that Qt will display warning here,
        // and there seems no way to suppress it.
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        // add database here
        QSqlDatabase db = QSqlDatabase::addDatabase(EPSIM_BACKEND_DB);
        db.setHostName(EPSIM_DB_HOSTNAME);
        db.setDatabaseName(networkDbFilePath);
        db.setUserName(EPSIM_DB_USERNAME);
        db.setPassword(EPSIM_DB_PASSWORD);
        if (!db.open()) {
	    QMessageBox::warning(0, QObject::tr("Database Error"),
				     QObject::tr("Cannot open Network "
						 "Database file. "
						 "Will use local "
						 "Database file. "
						 "Network storage is "
						 "disabled."));
	    // get rid of network storage until user fixes the problem
	    EpCore::clearFlag(options_->filePathFlags, 
			       Options::EnableNetworkStorage);
           
        }
        else
            return;
    }
    // use local database
    QString systemDbFilePath(EpCore::joinPaths(EpCore::systemPath(),
                                               EPSIM_DB_FILENAME));
    QSqlDatabase db = QSqlDatabase::addDatabase(EPSIM_BACKEND_DB);
    db.setHostName(EPSIM_DB_HOSTNAME);
    db.setDatabaseName(systemDbFilePath);
    db.setUserName(EPSIM_DB_USERNAME);
    db.setPassword(EPSIM_DB_PASSWORD);
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        throw EpCore::DatabaseError(db.databaseName());
    }
}

void AbstractMainWindow::systemSettings() {
    if (administrationAllowed()) {
        // store catalog in use since changing is expensive
        bool usingNetwork = options_->includeNetworkCatalog();
        SystemDialog systemDialog(options_,
            currentDisk()->studiesPath(), currentDisk()->label(),
            currentDisk()->translatedSide(), this);
        if (systemDialog.exec() == QDialog::Accepted) {
            systemDialog.setOptions();
            if (usingNetwork != options_->includeNetworkCatalog()) {
                qDebug() << "Using network changed!";
                changeDatabase();
            }
            updateSystemSettings();
            options_->save();
        }
    }
}

void AbstractMainWindow::help() {
    QUrl url(EpCore::directoryOf("doc").absoluteFilePath("index.html"));
    url.setScheme("file");
    QDesktopServices::openUrl(url);
}

void AbstractMainWindow::about() {
    QString buildVersion;
#ifdef APP_VERSION_BUILD
    buildVersion = tr("Build version %1<br />")
                   .arg(QString::fromLatin1(APP_VERSION_BUILD_STR));
#endif
    QString programName = QObject::tr("EP Simulator");
    QMessageBox::about(this, 
                       QObject::tr("About %1")
                       .arg(programName),
                       QObject::tr("<h2>%1 %2</h2>"
                       "Built on %4 at %5 (%9 bit)<br />"
                       "%6"
                       "<br />"
                       "EP Simulator simulates an EP recording "
                       "system.<br /><br />"
                       "Copyright &copy; 2006-%3 %10<br />"
                       "Licensed under the GNU GPL Version 2<br /><br />"
                       "<a href=http://www.epstudiossoftware.com> "
                       "http://www.epstudiossoftware.com</a>")
                       .arg(programName)
                       .arg(QLatin1String(EPSIM_VERSION))
                       .arg(QLatin1String(EPSIM_YEAR))
                       .arg(QLatin1String(__DATE__))
                       .arg(QLatin1String(__TIME__))
                       .arg(buildVersion)
                       .arg(QString::number(QSysInfo::WordSize))
                       .arg(QLatin1String(EPSIM_AUTHOR)));
}

void AbstractMainWindow::login() {
    if (!user_->isAdministrator()) {
        PasswordDialog pwDialog(options_->passwordHash, this);
        if (pwDialog.exec() == QDialog::Accepted) {
            user_->setIsAdministrator(true);
            updateAll();
        }
    }
}

void AbstractMainWindow::logout() {
    user_->setIsAdministrator(false);
    updateAll();
}

void AbstractMainWindow::changePassword() {
    if (administrationAllowed()) {
        ChangePasswordDialog cpDialog(options_->passwordHash, this);
        if (cpDialog.exec() == QDialog::Accepted) {
            options_->passwordHash = cpDialog.changePassword();
            options_->save();
        }
    }
}

void AbstractMainWindow::setIntervals() {
    if (administrationAllowed()) {
        EditIntervalsDialog d(this);
        d.exec();
    }
}

void AbstractMainWindow::setColumnFormats() {
    if (administrationAllowed()) {
        EditColumnFormatsDialog d(this);
        d.exec();
    }
}

void AbstractMainWindow::setProtocols() {
    if (administrationAllowed()) {
        EditProtocolsDialog d(this);
        d.exec();
    }
}

/// Checks to see if administrator access if required, if it is,
/// and not logged in as administrator, will do a login, then
/// will allow adminstration if user successfully logged in.
bool AbstractMainWindow::administrationAllowed() {
    if (!options_->administratorAccountRequired)
        return true;
    login();
    return user_->isAdministrator();
}

/// Shows the *Simulator Settings* menu item.
bool AbstractMainWindow::showSimulatorSettings() {
    return !options_->hideSimulatorMenu || user_->isAdministrator();
}

void AbstractMainWindow::updateWindowTitle(const QString& title) {
    const QString appTitle(QObject::tr("EP Simulator"));
    QString windowTitle = title.isEmpty() ? appTitle :
        QString("%1 %2").arg(appTitle).arg(title);
    windowTitle = user_->isAdministrator() ? 
        QString("%1 %2").arg(windowTitle).arg(QObject::tr("[Administrator]")) 
        : windowTitle;
    setWindowTitle(windowTitle);
}

void AbstractMainWindow::restore() {
    readSettings();
    show();
}

/// This is not for final production, just during development.
void AbstractMainWindow::filler() {
    QMessageBox::information(this, 
                             QObject::tr("FYI"),
                             QObject::tr("This function is not " 
                             "implemented yet."));
}

void AbstractMainWindow::createActions() {
    intervalsAction_= createAction(this, tr("Intervals"),
        tr("Intervals"), SLOT(setIntervals()));
    columnFormatsAction_= createAction(this, tr("Column Formats"),
        tr("Column formats"), SLOT(setColumnFormats()));
    protocolsAction_= createAction(this, tr("Protocols"),
        tr("Protocols"), SLOT(setProtocols()));
    loginAction_ = createAction(this, tr("Login..."),
        tr("Login as administrator"), SLOT(login()));
    logoutAction_ = createAction(this, tr("Logout"),
        tr("Logout from administrator"), SLOT(logout()));
    changePasswordAction_= createAction(this, tr("Change Password..."),
        tr("Change administrator password"), SLOT(changePassword()));
    aboutAction_ = createAction(this, tr("&About EP Simulator"),
        tr("About EP Simulator"), SLOT(about()));
    aboutQtAction_ = new QAction(tr("About &Qt"), this);
    aboutQtAction_->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    helpAction_ = createAction(this, tr("EP Simulator Help..."),
        tr("Get help for EP Simulator"), SLOT(help()), 
        QKeySequence::HelpContents);
    systemSettingsAction_ = createAction(this, tr("System Settings"),
        tr("Change system settings"), SLOT(systemSettings()));
    simulatorSettingsAction_ = createAction(this, tr("*Simulator Settings*"), 
        tr("Change simulator settings"), SLOT(simulatorSettings()));
}
