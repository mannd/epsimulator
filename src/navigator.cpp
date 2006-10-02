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

// Basically the navigator window is the main window.  When switching to
// the epsimulator window, we will actually just be changing the menus
// and the central widget
                            // main window

#include "navigator.h"
#include "epsimulator.h"    // some menu items are duplicated in the epsimulator
#include "study.h"
#include "patientdialog.h"

#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qmessagebox.h>
#include <qsplitter.h>
#include <qbuttongroup.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qsizepolicy.h>

Navigator::Navigator(QWidget* parent, const char* name)
 : QMainWindow( parent, name, WDestructiveClose ) {
    createActions();
    createMenus();
    createCentralWidget();

    setCaption(tr("EP Simulator Navigator"));
    setIcon(QPixmap::fromMimeSource("hi32-app-epsimulator.png"));

    statusBar()->message(tr("EP Simulator (c) 2006 EP Studios, Inc."));
}

void Navigator::createCentralWidget() {
    horizontalSplitter = new QSplitter(Horizontal, this);
    setCentralWidget(horizontalSplitter);
    buttonGroupView = new QButtonGroup(1, Horizontal, horizontalSplitter);
    ///TODO: need to do the width more elegantly
    buttonGroupView->setFixedWidth(200);
    buttonGroupView->setPaletteBackgroundColor("blue");
    
    newStudyButton = new QPushButton(QPixmap::fromMimeSource("hi32-newstudy.png"),
        "" , buttonGroupView);
    connect(newStudyButton, SIGNAL(clicked()), this, SLOT(newStudy()));    
    newStudyLabel = new QLabel(tr("New Study"), buttonGroupView);
    formatLabel(newStudyLabel, newStudyButton);
    continueStudyButton = new QPushButton(QPixmap::fromMimeSource("hi32-continuestudy.png"),
        "", buttonGroupView);
    continueStudyLabel = new QLabel(tr("Continue Study"), buttonGroupView);
    formatLabel(continueStudyLabel, continueStudyButton);
    reviewStudyButton = new QPushButton(QPixmap::fromMimeSource("hi32-reviewstudy.png"),
        "", buttonGroupView);
    reviewStudyLabel = new QLabel(tr("Review Study"), buttonGroupView);
    formatLabel(reviewStudyLabel, reviewStudyButton);
    preregisterPatientButton = 
        new QPushButton(QPixmap::fromMimeSource("hi32-preregister.png"),
        "", buttonGroupView);
    connect(preregisterPatientButton, SIGNAL(clicked()), this, SLOT(preregisterPatient()));
    preregisterPatientLabel = new QLabel(tr("Pre-Register Patient"), buttonGroupView);
    formatLabel(preregisterPatientLabel, preregisterPatientButton);
    reportsButton = new QPushButton(QPixmap::fromMimeSource("hi32-reports.png"),
        "", buttonGroupView);
    reportsLabel = new QLabel(tr("Reports"), buttonGroupView);
    formatLabel(reportsLabel, reportsButton);
    tableListView = new QListView(horizontalSplitter);
    tableListView->addColumn(tr("Patient"));
    tableListView->addColumn(tr("MRN"));
    tableListView->addColumn(tr("Study Date"));
    tableListView->addColumn(tr("Study Number"));
    tableListView->setAllColumnsShowFocus(true);
    tableListView->setShowSortIndicator(true);
    tableListView->setResizeMode(QListView::AllColumns);
}

void Navigator::formatLabel(QLabel* label, QPushButton* button) {
    label->setBuddy(button);
    label->setAlignment(Qt::AlignHCenter);
    label->setPaletteForegroundColor("white");
}

void Navigator::createActions() {
    newAct = new QAction(tr("&New..."), tr("Ctrl+N"), this);
    newAct->setIconSet(QPixmap::fromMimeSource("hi32-newstudy.png"));
    newAct->setStatusTip(tr("New study"));
    connect(newAct, SIGNAL(activated()), this, SLOT(newStudy()));

    continueAct = new QAction(tr("&Continue"), 0, this);
    continueAct->setIconSet(QPixmap::fromMimeSource("hi32-continuestudy.png"));
    continueAct->setStatusTip(tr("Continue study"));
    
    reviewAct = new QAction(tr("&Review"), 0, this);
    reviewAct->setIconSet(QPixmap::fromMimeSource("hi32-reviewstudy.png"));
    reviewAct->setStatusTip(tr("Review study"));

    preregisterAct = new QAction(tr("&Pre-Register"), 0, this);
    preregisterAct->setIconSet(QPixmap::fromMimeSource("hi32-preregister.png"));
    preregisterAct->setStatusTip(tr("Pre-register patient"));
    connect(preregisterAct, SIGNAL(activated()), this, SLOT(preregisterPatient()));

    reportsAct = new QAction(tr("R&eports..."), 0, this);
    reportsAct->setIconSet(QPixmap::fromMimeSource("hi32-reports.png"));
    reportsAct->setStatusTip(tr("Procedure reports"));

    copyAct = new QAction(tr("Copy..."), 0, this);
    copyAct->setStatusTip(tr("Copy study"));

    deleteAct = new QAction(tr("Delete..."), 0, this);
    deleteAct->setStatusTip(tr("Delete study"));

    exportAct = new QAction(tr("Export..."), 0, this);
    exportAct->setStatusTip(tr("Export study"));

    // Exit menu item is the only one with accelerator key
    exitAct = new QAction(tr("E&xit"), tr("Ctrl+X"), this);
    exitAct->setStatusTip("Exit EP Simulator");
    connect(exitAct, SIGNAL(activated()), this, SLOT(close()));

    loginAct = new QAction(tr("Login..."), 0, this);
    loginAct->setStatusTip(tr("Login"));

    logoutAct = new QAction(tr("Logout"), 0, this);
    logoutAct->setStatusTip(tr("Logout"));

    changePasswordAct = new QAction(tr("Change Password..."), 0, this);
    changePasswordAct->setStatusTip(tr("Change administrator password"));

    intervalsAct = new QAction(tr("Intervals"), 0, this);
    intervalsAct->setStatusTip(tr("Intervals"));

    columnFormatsAct = new QAction(tr("Column Formats"), 0, this);
    columnFormatsAct->setStatusTip(tr("Column formats"));

    protocolsAct = new QAction(tr("Protocols"), 0, this);
    protocolsAct->setStatusTip(tr("Protocols"));

    studyConfigurationsAct = new QAction(tr("Study Configurations"), 0, this);
    studyConfigurationsAct->setStatusTip(tr("Study configurations"));
   
    systemSettingsAct = new QAction(tr("System Settings"), 0, this);
    systemSettingsAct->setStatusTip(tr("Change system settings"));
 
    aboutAct = new QAction(tr("&About EP Simulator"), 0, this);
    aboutAct->setStatusTip(tr("About EP Simulator"));
    connect(aboutAct, SIGNAL(activated()), this, SLOT(about()));
}

void Navigator::createMenus() {

    studyMenu = new QPopupMenu(this);
    newAct->addTo(studyMenu);
    continueAct->addTo(studyMenu);
    reviewAct->addTo(studyMenu);
    preregisterAct->addTo(studyMenu);
    reportsAct->addTo(studyMenu);
    studyMenu->insertSeparator();
    copyAct->addTo(studyMenu);
    deleteAct->addTo(studyMenu);
    exportAct->addTo(studyMenu);
    studyMenu->insertSeparator();
    exitAct->addTo(studyMenu);

    catalogMenu = new QPopupMenu(this);
    utilitiesMenu = new QPopupMenu(this);

    administrationMenu = new QPopupMenu(this);
    securitySubMenu = new QPopupMenu(this);
    loginAct->addTo(securitySubMenu);
    logoutAct->addTo(securitySubMenu);
    changePasswordAct->addTo(securitySubMenu);
    administrationMenu->insertItem(tr("Security"), securitySubMenu);
    //insert Lists submenu here
    administrationMenu->insertSeparator();
    intervalsAct->addTo(administrationMenu);
    columnFormatsAct->addTo(administrationMenu);
    protocolsAct->addTo(administrationMenu);
    studyConfigurationsAct->addTo(administrationMenu);
    administrationMenu->insertSeparator();
    systemSettingsAct->addTo(administrationMenu);
    // insert reports submenu here

    helpMenu = new QPopupMenu(this);
    aboutAct->addTo(helpMenu);

    menuBar()->insertItem(tr("&Study"), studyMenu);
    menuBar()->insertItem(tr("&Catalog"), catalogMenu);
    menuBar()->insertItem(tr("&Utilities"), utilitiesMenu);
    menuBar()->insertItem(tr("&Administration"), administrationMenu);
    menuBar()->insertItem(tr("&Help"), helpMenu);

}

void Navigator::patientInformation() {
    Study newStudy(study_);
    PatientDialog* patientDialog = new PatientDialog(this);
    patientDialog->setFields(newStudy);
    if (patientDialog->exec()) {
        patientDialog->getFields(newStudy);
        study_ = newStudy;
    }
    studies_.push_back(study_);
    QListViewItem* item = new QListViewItem(tableListView);
    item->setText(0, study_.fullName());
//    studyTable->refresh(studies_);
}

void Navigator::newStudy() {
    patientInformation();
    startStudy(study_);
}

void Navigator::preregisterPatient() {
    patientInformation();
}

void Navigator::startStudy(Study& study) {
    
    // start up the main study screen
}

void Navigator::about() {
    Epsimulator::about(this);
}

void Navigator::closeEvent(QCloseEvent *event) {
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

Navigator::~Navigator() {
}
