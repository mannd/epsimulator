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

#include "actions.h"

#include "changepassworddialog.h"
#include "options.h"
#include "passworddialog.h"
#include "passwordhandler.h"
#include "user.h"
#include "versioninfo.h"

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QStringList>

/**
 * @namespace EpGui program functions that require QtGui
 */
namespace EpGui {

using EpCore::Options;
using EpCore::PasswordHandler;
using EpCore::User;
using EpCore::VersionInfo;

bool login(QWidget* parent, User* user) {
    bool success = false;
    if (!user->isAdministrator()) {
        PasswordDialog pwDialog(parent);
        if (pwDialog.exec() == QDialog::Accepted) {
            user->makeAdministrator(true);
            success = true;
        }
    }
    return success;
}

void logout(User* user) {
    user->makeAdministrator(false);
}

void changePassword(QWidget* parent) {
    ChangePasswordDialog cpDialog(parent);
    if (cpDialog.exec() == QDialog::Accepted) 
        cpDialog.changePassword();
}

bool showSimulatorSettings(Options* options, User* user) {
    return !options->hideSimulatorMenu || user->isAdministrator();
}

void about(QWidget* parent) {
    VersionInfo* info = VersionInfo::instance();
    QMessageBox::about(parent, 
                       QObject::tr("About %1")
                       .arg(info->programName()),
                       QObject::tr("<h2>%1 %2</h2>"
                       "<p>Copyright &copy; %3 EP Studios, Inc."
                       "<p>EP Simulator simulates an EP recording "
                       "system."
                       "<p><a href=http://www.epstudiossoftware.com> "
                       "http://www.epstudiossoftware.com</a>")
                       .arg(info->programName()).arg(info->version())
                       .arg(info->copyrightYear()));
}

void help(QWidget* parent) {
    QMessageBox::information(parent, 
                             QObject::tr("%1 Help")
                             .arg(VersionInfo::instance()->programName()),
                             QObject::tr("Help is available from "
                             "<p><a href=http://www.epstudiossoftware.com> "
                             "http://www.epstudiossoftware.com</a>"),
                             QMessageBox::Ok);
}

/// This is not for final production, just during development.
void filler(QWidget* widget) {
    QMessageBox::information(widget, 
                             QObject::tr("FYI"),
                             QObject::tr("This function is not " 
                             "implemented yet."));
}

/**
 * Opens a file dialog and provides a file for the lineEdit.
 * @param parent calling QDialog.
 * @param lineEdit function modifies the Text property of the LineEdit
 * @param defaultPath uses this path if lineEdit text is empty.  Avoids
 * random paths appearing, but is an optional parameter.
 */
void browseFilePaths(QWidget* parent, QLineEdit* lineEdit,
                     const QString& defaultPath) {
    QString initialPath = defaultPath;
    if (!lineEdit->text().isEmpty())
        initialPath = lineEdit->text();
    QFileDialog fd(parent, QObject::tr("Select Directory"),
                   initialPath);
    fd.setFileMode(QFileDialog::DirectoryOnly);
    if (fd.exec() == QDialog::Accepted) {
        QStringList files = fd.selectedFiles();
        QString fileName = QString();
        if (!files.isEmpty())
            fileName = files[0];
        lineEdit->setText(QDir::toNativeSeparators(fileName));
    }
}

/**
 * Sets up icon, status tip, and slot for an action.  Note that
 * a slot must be passed as a const char*.
 * @param w calling QWidget. 
 * @param name name of action.
 * @param statusTip status tip.
 * @param slotName name of slot to be signaled by triggered(bool).
 * @param accelKey shortcut key,if any.
 * @param iconName icon, if any.  Just the name, no path.
 * @return pointer to the created QAction.
 */
QAction* createAction(QWidget* w,
                      const QString& name,
                      const QString& statusTip,
                      const char* slotName,
                      const QKeySequence& accelKey,
                      const QString& iconName) {
    QAction* action = new QAction(name, w);
    if (!iconName.isEmpty())
        action->setIcon(QIcon(":/images/" + iconName));
    if (!accelKey.isEmpty())
        action->setShortcut(QKeySequence(accelKey));
    action->setStatusTip(statusTip);
    if (slotName)
        QObject::connect(action, SIGNAL(triggered(bool)), w, slotName);
    return action;
}

}
