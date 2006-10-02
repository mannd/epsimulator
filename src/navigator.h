/***************************************************************************
 *   Copyright (C) 2006 by EP Studios, Inc.                                *
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
#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "study.h"

#include <qmainwindow.h>

class QAction;
class QPopupMenu;
class QSplitter;
class QButtonGroup;
class QListView;
class QButton;

/**
	@author David Mann <mannd@epstudiossoftware.com>
*/
class Navigator : public QMainWindow
{
    Q_OBJECT

public:
    Navigator(QWidget* parent = 0, const char *name = "navigator");
    ~Navigator();

protected:
    void closeEvent(QCloseEvent * event);
    //    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    void patientInformation();
    /*    void openPatient();
        void newStudy();
        void openStudy();
        // Patients and studies are saved automatically when created and modified
        // At least I think that is the Prucka behavior
        void about();
    */

    void about();

private:
    void createActions();
    void createMenus();

    // current study
    // it looks like in Qt subclasses, embedded objects must be pointers
    Study study_;
    Studies studies_;

    // central widget stuff    QSplitter* horizontalSplitter;
    QSplitter* horizontalSplitter;
    QButtonGroup* buttonGroupView;
    QListView* tableListView;
    QButton* newStudyButton;
    QButton* continueStudyButton;
    QButton* reviewStudyButton;
    QButton* preregisterPatientButton;
    QButton* reportsButton; 

    // Study menu
    QAction *newAct;
    QAction *continueAct;
    QAction *reviewAct;
    QAction *preregisterAct;
    QAction *reportsAct;
    QAction *copyAct;
    QAction *deleteAct;
    QAction *exportAct;
    QAction *exitAct;

    // Administration menu
    QAction *loginAct;
    QAction *logoutAct;
    QAction *changePasswordAct;
    QAction *intervalsAct;
    QAction *columnFormatsAct;
    QAction *protocolsAct;
    QAction *studyConfigurationsAct;
    QAction *systemSettingsAct;


    QAction *aboutAct;

    QPopupMenu *studyMenu;
    QPopupMenu *catalogMenu;
    QPopupMenu *utilitiesMenu;
    QPopupMenu *administrationMenu;
    QPopupMenu *securitySubMenu;
    QPopupMenu *helpMenu;
};

#endif
