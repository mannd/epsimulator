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
#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include "displaywindow.h"
#include "recorderdefs.h"

#include <QTableView>

class QAction;
class QComboBox;
class QSettings;

namespace EpRecorder {

/**
Horizontally situation window at the bottom of the Central Widget, for logging events.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class LogWindow : public DisplayWindow  {
    Q_OBJECT
public:
    LogWindow(int number = 0, QWidget *parent = 0);

    virtual void writeSettings(QSettings&);
    virtual void readSettings(QSettings&);
    virtual QString key() {return EpRecorder::logWindowKey;}

    ~LogWindow();

private:
    void createActions();
    void createToolBars();

    QComboBox* logFilterComboBox_;

    QAction* updateLogWindowAction_;
    QAction* firstLogEntryAction_;
    QAction* lastLogEntryAction_;
    QAction* undoAction_;
    QAction* redoAction_;
    QAction* createEventAction_;
    QAction* printAction_;
};

}

#endif
