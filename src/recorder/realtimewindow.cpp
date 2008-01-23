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

#include "realtimewindow.h"

#include "actions.h"

#include <QAction>
#include <QIcon>
#include <QPalette>
#include <QToolBar>

using EpGui::createAction;

RealTimeWindow::RealTimeWindow(QWidget* parent)
 : QMainWindow(parent) {
    setWindowTitle(tr("Real-Time Page 1"));
    QWidget* centralWidget = new QWidget;
    centralWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    centralWidget->setPalette(palette);
    setCentralWidget(centralWidget);
    /// FIXME this is just a test toolbar
    QToolBar* toolBar = new QToolBar(this);
    toolBar->setAutoFillBackground(true);
    QAction* testAction = createAction(this, QString("test"), QString("test"));
    toolBar->addAction(testAction);
    toolBar->addSeparator();
    
    
}


RealTimeWindow::~RealTimeWindow()
{
}


