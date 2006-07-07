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

#include <kmainwindow.h>
#include <klocale.h>
#include <kstatusbar.h>
#include <kpopupmenu.h>
#include <kmenubar.h>

Epsimulator::Epsimulator()
    : KMainWindow( 0, "epsimulator" )
{
    KPopupMenu *menuStudy = new KPopupMenu(this);
    KPopupMenu *menuStudyConfiguration = new KPopupMenu(this);
    KPopupMenu *menuMeasurements = new KPopupMenu(this);
    KPopupMenu *menuWindows = new KPopupMenu(this);
    KPopupMenu *menuAdministration = new KPopupMenu(this);

    menuBar()->insertItem(i18n("&Study"), menuStudy);
    menuBar()->insertItem(i18n("Study &Configuration"), menuStudyConfiguration);
    menuBar()->insertItem(i18n("&Measurements"), menuMeasurements);
    menuBar()->insertItem(i18n("&Windows"), menuWindows);
    menuBar()->insertItem(i18n("&Administration"), menuAdministration);
    
    menuBar()->insertItem(i18n("&Help"), helpMenu());

    statusBar()->message(i18n("EPSimulator has started."));

}

Epsimulator::~Epsimulator()
{
}

void Epsimulator::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::question(
            this,
            i18n("Really quit?"),
            i18n("Quit EPSimulator?"),
            QMessageBox::Yes | QMessageBox::Default,
            QMessageBox::No,
            QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

#include "epsimulator.moc"
