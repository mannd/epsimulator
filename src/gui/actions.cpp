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

#include "versioninfo.h"

#include <QAction>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QStringList>
#include <QWidget>

/// @namespace EpGui program functions that require QtGui
namespace EpGui {

void about(QWidget* parent) {
    VersionInfo* info = VersionInfo::instance();
    QMessageBox::about(parent, QObject::tr("About %1").arg(info->programName()),
		       QObject::tr("<h2>%1 %2</h2>"
		          "<p>Copyright &copy; %3 EP Studios, Inc."
			  "<p>EP Simulator simulates an EP recording "
			  "system."
                          "<p><a href=http://www.epstudiossoftware.com> "
                          "http://www.epstudiossoftware.com</a>")
                          .arg(info->programName()).arg(info->version())
                          .arg(info->copyrightYear()));
///TODO make this an actual hyperlink that you can click on and go to
}

void help(QWidget* parent) {
    QMessageBox::information(parent, QObject::tr(
        "%1 Help").arg(VersionInfo::instance()->programName()),
        QObject::tr(
        "Help is available from "
        "<p><a href=http://www.epstudiossoftware.com> "
        "http://www.epstudiossoftware.com</a>"),
        QMessageBox::Ok);
}

/// This is not for final production, just during development.
void filler(QWidget* widget) {
    QMessageBox::information(widget, QObject::tr("FYI"),
                             QObject::tr("This function is not implemented yet."));
}

/**
 * 
 * @param parent 
 * @param lineEdit function modifies the Text property of the LineEdit
 * @param defaultPath uses this path if lineEdit text is empty.  Avoids
 * random paths appearing, but is an optional parameter.
 */
void browseFilePaths(QWidget* parent, QLineEdit* lineEdit,
    const QString& defaultPath) {
    QString initialPath = defaultPath;
    if (!lineEdit->text().isEmpty())
        initialPath = lineEdit->text();
    QFileDialog *fd = new QFileDialog(parent, 
        QObject::tr("Select Directory"), initialPath);
    fd->setFileMode(QFileDialog::DirectoryOnly);
    if (fd->exec() == QDialog::Accepted) {
        QStringList files = fd->selectedFiles();
        QString fileName = QString();
        if (!files.isEmpty())
            fileName = files[0];
        lineEdit->setText(fileName);
    }
}

/**
 * Sets up icon, status tip, and slot for an action.
 */
void setupAction(QAction* action, 
		 QWidget* w,
		 const QString& statusTip,
		 const char* slotName,
                 const QString& accelKey,
                 const QString& iconName ) {
    if (!iconName.isEmpty())
        action->setIcon(QIcon(iconName));
    if (!accelKey.isEmpty())
        action->setShortcut(accelKey);
    action->setStatusTip(statusTip);
    if (slotName)
        QObject::connect(action, SIGNAL(activated()), w, slotName);
} 

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
        QObject::connect(action, SIGNAL(triggered()), w, slotName);
    return action;
}

}
