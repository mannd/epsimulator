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
#include "neweditintervalsdialog.h"
#include "editcolumnformatsdialog.h"
#include "editprotocolsdialog.h"
#include "fileutilities.h"
#include "opticaldisk.h"
#include "options.h"
#include "passworddialog.h"
#include "simulatorsettingsdialog.h"
#include "systemdialog.h"
#include "user.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

using EpCore::Options;
using EpGui::AbstractMainWindow;
using EpHardware::EpOpticalDisk::OpticalDisk;
using namespace EpCore::Constants;

AbstractMainWindow::AbstractMainWindow(QWidget *parent)
    : QMainWindow(parent) {
    createActions();
}

AbstractMainWindow::~AbstractMainWindow() {}

void AbstractMainWindow::simulatorSettings() {
    if (administrationAllowed()) {
        SimulatorSettingsDialog simDialog(options(), this);
        if (simDialog.exec() == QDialog::Accepted) {
            simDialog.setOptions();
            updateSimulatorSettings();
            options()->save();
        }
    }
}

void AbstractMainWindow::systemSettings() {
    if (administrationAllowed()) {
        SystemDialog systemDialog(options(),
            currentDisk()->studiesPath(), currentDisk()->label(),
            currentDisk()->translatedSide(), this);
        if (systemDialog.exec() == QDialog::Accepted) {
            systemDialog.setOptions();
            updateSystemSettings();
            options()->save();
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
    if (!user()->isAdministrator()) {
        PasswordDialog pwDialog(options()->passwordHash, this);
        if (pwDialog.exec() == QDialog::Accepted) {
            user()->setIsAdministrator(true);
            updateAll();
        }
    }
}

void AbstractMainWindow::logout() {
    user()->setIsAdministrator(false);
    updateAll();
}

void AbstractMainWindow::changePassword() {
    if (administrationAllowed()) {
        ChangePasswordDialog cpDialog(options()->passwordHash, this);
        if (cpDialog.exec() == QDialog::Accepted) {
            options()->passwordHash = cpDialog.changePassword();
            options()->save();
        }
    }
}

void AbstractMainWindow::setIntervals() {
    if (administrationAllowed()) {
        NewEditIntervalsDialog d(this);
        d.exec();
        // if (d.exec()) {
        //     d.intervals().update();
        //}
    }
}

void AbstractMainWindow::setColumnFormats() {
    if (administrationAllowed()) {
        EditColumnFormatsDialog d(this);
        d.exec();
    //     // if (d.exec()) {
    //     //     d.columnFormats().update();
    //     // }
    }
}

void AbstractMainWindow::setProtocols() {
    // if (administrationAllowed()) {
    //     EditProtocolsDialog d(this);
    //     d.exec();
    //     // if (d.exec()) {
    //     //     d.protocols().update();
    //     // }
    //    }
}

/// Checks to see if administrator access if required, if it is,
/// and not logged in as administrator, will do a login, then
/// will allow adminstration if user successfully logged in.
bool AbstractMainWindow::administrationAllowed() {
    if (!options()->administratorAccountRequired)
        return true;
    login();
    return user()->isAdministrator();
}

/// Shows the *Simulator Settings* menu item.
bool AbstractMainWindow::showSimulatorSettings() {
    return !options()->hideSimulatorMenu || user()->isAdministrator();
}

void AbstractMainWindow::updateWindowTitle(const QString& title) {
    const QString appTitle(QObject::tr("EP Simulator"));
    QString windowTitle = title.isEmpty() ? appTitle :
        QString("%1 %2").arg(appTitle).arg(title);
    windowTitle = user()->isAdministrator() ? 
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
