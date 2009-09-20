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

#include "options.h"

#include <QAction>
#include <QFileDialog>
#include <QLineEdit>
#include <QObject>

/**
 * @namespace EpGui program functions that require QtGui
 */
namespace EpGui {

using EpCore::Options;

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
