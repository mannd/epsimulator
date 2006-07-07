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
#include <qtable.h>		// this is a tmp central widget

Epsimulator::Epsimulator()
    : QMainWindow( 0, "epsimulator", WDestructiveClose )
{
    qtable = new QTable(this);
    setCentralWidget(qtable);

    QPopupMenu *menuStudy = new QPopupMenu(this);
    QPopupMenu *menuStudyConfiguration = new QPopupMenu(this);
    QPopupMenu *menuMeasurements = new QPopupMenu(this);
    QPopupMenu *menuWindows = new QPopupMenu(this);
    QPopupMenu *menuAdministration = new QPopupMenu(this);

    menuBar()->insertItem(tr("&Study"), menuStudy);
    menuBar()->insertItem(tr("Study &Configuration"), menuStudyConfiguration);
    menuBar()->insertItem(tr("&Measurements"), menuMeasurements);
    menuBar()->insertItem(tr("&Windows"), menuWindows);
    menuBar()->insertItem(tr("&Administration"), menuAdministration);
    
    menuBar()->insertItem(tr("&Help"), helpMenu());

    statusBar()->message(tr("EPSimulator has started."));

}

Epsimulator::~Epsimulator()
{
}

void Epsimulator::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::question(
            this,
            tr("Really quit?"),
            tr("Quit EPSimulator?"),
            QMessageBox::Yes | QMessageBox::Default,
            QMessageBox::No,
            QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

#include "epsimulator.moc"
