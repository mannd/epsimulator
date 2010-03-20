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
#include "editintervalsdialog.h"
#include "editcolumnformatsdialog.h"
#include "editprotocolsdialog.h"
#include "fileutilities.h"
#include "opticaldisk.h"
#include "options.h"
#include "passworddialog.h"
#include "simulatorsettingsdialog.h"
#include "systemdialog.h"
#include "user.h"
#include "versioninfo.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

using EpCore::Options;
using EpCore::VersionInfo;
using EpGui::AbstractMainWindow;
using EpHardware::EpOpticalDisk::OpticalDisk;

AbstractMainWindow::AbstractMainWindow(QWidget *parent)
    : QMainWindow(parent), versionInfo_(VersionInfo::instance()) {
    createActions();
}

AbstractMainWindow::~AbstractMainWindow() {}

void AbstractMainWindow::simulatorSettings() {
    if (administrationAllowed()) {
        SimulatorSettingsDialog simDialog(this);
        if (simDialog.exec() == QDialog::Accepted) {
            simDialog.setOptions();
            updateSimulatorSettings();
        }
    }
}

void AbstractMainWindow::systemSettings() {
    if (administrationAllowed()) {
        SystemDialog systemDialog(
            currentDisk()->studiesPath(), currentDisk()->label(),
            currentDisk()->translatedSide(), this);
        if (systemDialog.exec() == QDialog::Accepted) {
            systemDialog.setOptions();
            updateSystemSettings();
        }
    }
}

void AbstractMainWindow::help() {
    QUrl url(EpCore::directoryOf("doc").absoluteFilePath("index.html"));
    url.setScheme("file");
    QDesktopServices::openUrl(url);
}

void AbstractMainWindow::about() {
    QMessageBox::about(this, 
                       QObject::tr("About %1")
                       .arg(versionInfo_->programName()),
                       QObject::tr("<h2>%1 %2</h2>"
                       "<p>Copyright &copy; %3 EP Studios, Inc."
                       "<p>EP Simulator simulates an EP recording "
                       "system."
                       "<p><a href=http://www.epstudiossoftware.com> "
                       "http://www.epstudiossoftware.com</a>")
                       .arg(versionInfo_->programName())
                       .arg(versionInfo_->version())
                       .arg(versionInfo_->copyrightYear()));
}

void AbstractMainWindow::login() {
    if (!user()->isAdministrator()) {
        PasswordDialog pwDialog(this);
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
        ChangePasswordDialog cpDialog(this);
        if (cpDialog.exec() == QDialog::Accepted) 
            cpDialog.changePassword();
    }
}

void AbstractMainWindow::setIntervals() {
    if (administrationAllowed()) {
        EditIntervalsDialog d(this);
        if (d.exec()) {
            d.intervals().update();
        }
    }
}

void AbstractMainWindow::setColumnFormats() {
    if (administrationAllowed()) {
        EditColumnFormatsDialog d(this);
        if (d.exec()) {
            d.columnFormats().update();
        }
    }
}

void AbstractMainWindow::setProtocols() {
    if (administrationAllowed()) {
        EditProtocolsDialog d(this);
        if (d.exec()) {
            //d.protocols().update();
        }
    }
}

/// Checks to see if administrator access if required, if it is,
/// and not logged in as administrator, will do a login, then
/// will allow adminstration if user successfully logged in.
bool AbstractMainWindow::administrationAllowed() {
    if (!epOptions->administratorAccountRequired)
        return true;
    login();
    return user()->isAdministrator();
}

/// Shows the *Simulator Settings* menu item.
bool AbstractMainWindow::showSimulatorSettings() {
    return !epOptions->hideSimulatorMenu || user()->isAdministrator();
}

void AbstractMainWindow::updateWindowTitle(const QString& title) {
    QString windowTitle = title.isEmpty() ? VersionInfo::instance()->programName() :
        QString("%1 %2").arg(VersionInfo::instance()->programName()).arg(title);
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
