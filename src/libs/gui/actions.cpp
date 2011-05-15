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

namespace EpGui {

using EpCore::Options;

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
