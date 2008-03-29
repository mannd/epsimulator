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

#ifndef ACTIONS_H
#define ACTIONS_H

#include <QDir>
#include <QKeySequence>
#include <QString>

class Options;
class QAction;
class QLineEdit;
class QWidget;
class User;

namespace EpGui {

bool login(QWidget*, User*);
void logout(User*);
void changePassword(QWidget*, Options*);
bool showSimulatorSettings(Options*, User*);
void about(QWidget*);
void help(QWidget*);

// Gives "not implemented yet" error message.
void filler(QWidget*);

void browseFilePaths(QWidget*, QLineEdit*, const QString& defaultPath = 
    QDir::homePath());

QAction* createAction(QWidget*,
		      const QString& name,
		      const QString& statusTip,
                      const char* slotName = 0,
		      const QKeySequence& accelKey 
			= QKeySequence(),
                      const QString& iconName 
			= QString());

}

#endif




