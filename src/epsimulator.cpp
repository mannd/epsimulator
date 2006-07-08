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


#include "epsimulator.h"

#include <qlabel.h>
#include <qmessagebox.h>

#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qtable.h>		// this is a tmp central widget

Epsimulator::Epsimulator()
    : QMainWindow( 0, "epsimulator", WDestructiveClose )
{
    qtable = new QTable(this);
    setCentralWidget(qtable);

    createActions();
    createMenus();

    setCaption(tr("EP Simulator"));



    statusBar()->message(tr("EP Simulator has started."));

}

Epsimulator::~Epsimulator()
{
}

void Epsimulator::newPatient()
{
}

void Epsimulator::about()
{
    QMessageBox::about(this, tr("About EP Simulator"),
		       tr("<h2>EP Simulator 0.1</h2>"
		          "<p>Copyright &copy; 2006 EP Studios, Inc."
			  "<p>EP Simulator simulates an EP recording "
			  "system."));
}

void Epsimulator::createActions()
{
    newPatientAct = new QAction(tr("&New Patient"), tr("Ctrl+N"), this);
    newPatientAct->setStatusTip(tr("Create a new patient"));
    connect(newPatientAct, SIGNAL(activated()), this, SLOT(newPatient()));

    exitAct = new QAction(tr("E&xit"), tr("Ctrl+X"), this);
    exitAct->setStatusTip(tr("Exit EP Simulator"));
    connect(exitAct, SIGNAL(activated()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), 0, this);
    aboutAct->setStatusTip(tr("About EP Simulator"));
    connect(aboutAct, SIGNAL(activated()), this, SLOT(about()));
}

void Epsimulator::createMenus()
{
//    QPopupMenu *menuStudy = new QPopupMenu(this);

    menuStudy = new QPopupMenu(this);
    newPatientAct->addTo(menuStudy);
    exitAct->addTo(menuStudy);

    menuStudyConfiguration = new QPopupMenu(this);
    menuMeasurements = new QPopupMenu(this);
    menuWindows = new QPopupMenu(this);
    menuAdministration = new QPopupMenu(this);
    menuHelp = new QPopupMenu(this);
    aboutAct->addTo(menuHelp);
    
    menuBar()->insertItem(tr("&Study"), menuStudy);
    menuBar()->insertItem(tr("Study &Configuration"), menuStudyConfiguration);
    menuBar()->insertItem(tr("&Measurements"), menuMeasurements);
    menuBar()->insertItem(tr("&Windows"), menuWindows);
    menuBar()->insertItem(tr("&Administration"), menuAdministration);
    menuBar()->insertSeparator();
    menuBar()->insertItem(tr("&Help"), menuHelp);
}

void Epsimulator::closeEvent(QCloseEvent *event)
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

