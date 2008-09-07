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
#ifndef ABSTRACTMAINWINDOW_H
#define ABSTRACTMAINWINDOW_H

#include <QMainWindow>

namespace EpCore {
    class User;
    class VersionInfo;
}

using EpCore::User;
using EpCore::VersionInfo;

namespace EpGui {

/**
Contains methods and actions common to both the Navigator and Recorder windows.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class AbstractMainWindow : public QMainWindow {
    Q_OBJECT
public:
    AbstractMainWindow(QWidget *parent = 0);

    ~AbstractMainWindow();

protected:
    virtual User* user() = 0;
    void filler();

protected slots:
    void about();
    void help();

private:
    const VersionInfo* versionInfo_;
};

}

#endif
