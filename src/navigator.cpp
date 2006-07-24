/***************************************************************************
 *   Copyright (C) 2006 by David Mann   *
 *   mannd@epstudiossoftware.com   *
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
#include "navigator.h"
#include "epsimulator.h"    // some menu items are duplicated in the epsimulator
                            // main window

#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qtable.h>
#include <qmessagebox.h>

Navigator::Navigator()
 : QMainWindow( 0, "navigator", WDestructiveClose )
{
    qtable = new QTable(this);
    setCentralWidget(qtable);

    createActions();
    createMenus();

    setCaption(tr("Navigator"));
    setIcon(QPixmap::fromMimeSource("hi16-app-epsimulator.png"));



    statusBar()->message(tr("EP Simulator (c) 2006 EP Studios, Inc."));
}

void Navigator::createActions()
{
    aboutAct = new QAction(tr("&About EP Simulator"), 0, this);
    aboutAct->setStatusTip(tr("About EP Simulator"));
    connect(aboutAct, SIGNAL(activated()), this, SLOT(about()));
}

void Navigator::createMenus()
{
    menuHelp = new QPopupMenu(this);
    aboutAct->addTo(menuHelp);

    menuBar()->insertItem(tr("Help"), menuHelp);

}

void Navigator::about()
{
    Epsimulator::about(this);
}

void Navigator::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::question(
            this,
            tr("Really quit?"),
            tr("Quit EP Simulator?"),
            QMessageBox::Yes | QMessageBox::Default,
            QMessageBox::No,
            QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

Navigator::~Navigator()
{
}


