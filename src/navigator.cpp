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
#include "systemdialog.h"
#include "studyconfigdialog.h"

#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qmessagebox.h>
#include <qsplitter.h>
#include <qlistview.h>
#include <qheader.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qsizepolicy.h>
#include <qsettings.h>

#include <algorithm>

#define BUTTON_SIZE 80

Navigator::Navigator(QWidget* parent, const char* name)
 : QMainWindow( parent, name, WDestructiveClose ) {
    ///TODO get this from the system options
    studiesPath_ = ".";

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

    buttonFrame = new QFrame(horizontalSplitter);
    buttonFrame->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, buttonFrame->sizePolicy().hasHeightForWidth() ) );
    buttonFrame->setFrameShape(QFrame::StyledPanel);
    buttonFrame->setPaletteBackgroundColor("blue");
    buttonFrame->setMaximumWidth(200);
    buttonFrameLayout = new QGridLayout(buttonFrame, 1, 1, 11, 6, "");

    newStudyButton = new QPushButton(buttonFrame);
    newStudyButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    newStudyButton->setPixmap(QPixmap::fromMimeSource("hi64-newstudy.png"));
    buttonFrameLayout->addWidget(newStudyButton, 0, 0);
//    buttonGridLayout->addWidget(newStudyButton, 0, 0);
    connect(newStudyButton, SIGNAL(clicked()), this, SLOT(newStudy())); 

    newStudyLabel = new QLabel(tr("New\nStudy"), buttonFrame);
    newStudyLabel->setPaletteForegroundColor("white");
    newStudyLabel->setAlignment( int( QLabel::AlignCenter ) );
    buttonFrameLayout->addWidget(newStudyLabel, 1, 0);

    continueStudyButton = new QPushButton(buttonFrame);
    continueStudyButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    continueStudyButton->setPixmap(QPixmap::fromMimeSource("hi64-continuestudy.png"));
    buttonFrameLayout->addWidget(continueStudyButton, 2, 0);
    
    continueStudyLabel = new QLabel(tr("Continue\nStudy"), buttonFrame);
    continueStudyLabel->setPaletteForegroundColor("white");
    continueStudyLabel->setAlignment( int( QLabel::AlignCenter ) );
    buttonFrameLayout->addWidget(continueStudyLabel, 3, 0);
 //   buttonGridLayout->addWidget(continueStudyButton, 1, 0);
    
    reviewStudyButton = new QPushButton(buttonFrame);
    reviewStudyButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    reviewStudyButton->setPixmap(QPixmap::fromMimeSource("hi64-reviewstudy.png"));
    buttonFrameLayout->addWidget(reviewStudyButton, 4, 0);
//    buttonGridLayout->addWidget(reviewStudyButton, 2, 0);
    
    reviewStudyLabel = new QLabel(tr("Review\nStudy"), buttonFrame);
    reviewStudyLabel->setPaletteForegroundColor("white");
    reviewStudyLabel->setAlignment( int( QLabel::AlignCenter ) );
    buttonFrameLayout->addWidget(reviewStudyLabel, 5, 0);
    
    preregisterPatientButton = new QPushButton(buttonFrame);
    preregisterPatientButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    preregisterPatientButton->setPixmap(QPixmap::fromMimeSource("hi64-preregister.png")); buttonFrameLayout->addWidget(preregisterPatientButton, 6, 0);
//    buttonGridLayout->addWidget(preregisterPatientButton, 3, 0);
    connect(preregisterPatientButton, SIGNAL(clicked()), this, SLOT(preregisterPatient()));

    preregisterPatientLabel = new QLabel(tr("Pre-\nRegister"), buttonFrame);
    preregisterPatientLabel->setPaletteForegroundColor("white");
    preregisterPatientLabel->setAlignment( int( QLabel::AlignCenter ) );
    buttonFrameLayout->addWidget(preregisterPatientLabel, 7, 0);
  
    reportsButton = new QPushButton(buttonFrame);
    reportsButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    reportsButton->setPixmap(QPixmap::fromMimeSource("hi64-reports.png"));
    buttonFrameLayout->addWidget(reportsButton, 8, 0);
    
    reportsLabel = new QLabel(tr("Reports"), buttonFrame);
    reportsLabel->setPaletteForegroundColor("white");
    reportsLabel->setAlignment( int( QLabel::AlignCenter ) );
    buttonFrameLayout->addWidget(reportsLabel, 9, 0);

    spacer = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    buttonFrameLayout->addItem( spacer, 10, 0 );

    tableListView = new QListView(horizontalSplitter);
    tableListView->addColumn(tr("Patient"));
    tableListView->addColumn(tr("MRN"));
    tableListView->addColumn(tr("Study Date/Time"));
    tableListView->addColumn(tr("Study Config"));
    tableListView->addColumn(tr("Study Number"));
    tableListView->addColumn(tr("Location of Study"));
    tableListView->addColumn(tr("Hidden key"));
    // hide the hidden key column: make sure it is defined correctly in KEY_COLUMN
    tableListView->setAllColumnsShowFocus(true);
    tableListView->setShowSortIndicator(true);
    tableListView->setColumnWidthMode(KEY_COLUMN, QListView::Manual);
    tableListView->hideColumn(KEY_COLUMN);
    tableListView->header()->setResizeEnabled(false, KEY_COLUMN);    
    //tableListView->setResizeMode(QListView::AllColumns);  
    // his messes up the hidden column
    readSettings(); 
    // populate the list from disk
    //loadStudies();
    populateTableListView();
}


/// This function populates the table from studies_.  studies_ is populated
/// in the Navigator constructor.
void Navigator::populateTableListView() {
    Studies::iterator pos;
    for (pos = studies_.begin(); pos != studies_.end(); ++pos) {
        (void) new QListViewItem(tableListView, 
            pos->second.name().fullName(),
            pos->second.mrn(),
            pos->second.dateTime().toString(),
            pos->second.config(),
            pos->second.number(),
            pos->second.path(),
            pos->second.key());
    }
}

void Navigator::saveSettings() {
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");

    QString str;
    QTextOStream out1(&str);
    out1 << *horizontalSplitter;
    settings.writeEntry("/horizontalSplitter", str);
    QTextOStream out2(&str);
    out2 << study_.path();
    settings.writeEntry("/studyPath", str);
    settings.endGroup();
}

void Navigator::readSettings() {
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");
    
    QString str1 = settings.readEntry("/horizontalSplitter");
    QTextIStream in1(&str1);
    in1 >> *horizontalSplitter;
    QString str2 = settings.readEntry("/studyPath");
    study_.setPath(str2);
    settings.endGroup();
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
    connect(systemSettingsAct, SIGNAL(activated()), this, SLOT(systemSettings()));
 
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

// returns true if PatientDialog is saved, false if cancelled
bool Navigator::getStudyInformation() {
    Study newStudy(study_);
    newStudy.setDateTime(QDateTime::currentDateTime());
    PatientDialog* patientDialog = new PatientDialog(this);
    patientDialog->setFields(newStudy);
    if (patientDialog->exec()) {
        patientDialog->getFields(newStudy);
        study_ = newStudy;  
        studies_[study_.key()] = study_;
        QListViewItem* item = new QListViewItem(tableListView);
        item->setText(0, study_.name().fullName());
        item->setText(1, study_.mrn());
        item->setText(2, study_.dateTime().toString());
        item->setText(3, study_.config());
        item->setText(4, study_.number());
        item->setText(5, study_.path());
        item->setText(6, study_.key());
        return true;
    }
    return false;
}

void Navigator::newStudy() {
///TODO study_ must be "blank" unless a study is selected in the catalog.
/// Same thing for preregister and continue study
    StudyConfigDialog* studyConfigDialog  = new StudyConfigDialog(this);
    if (studyConfigDialog->exec()) {
///TODO StudyConfigDialog should probably be SelectConfigDialog and 
/// need to fix below
//        study_.setConfig(studyConfigDialog->configListBox->currentText());
        if (getStudyInformation())
            startStudy();
    }
}

void Navigator::preregisterPatient() {
    getStudyInformation();
}

/// returns true and switches to study highlighted in the catalog; otherwise returns /// false if no study highlighted and leaves study_ unchanged
bool Navigator::studySelected() {
    if (QListViewItem* item = tableListView->selectedItem()) {
        Studies::iterator pos;
        pos = studies_.find(item->text(KEY_COLUMN));
        if (pos != studies_.end()) {
            study_ = pos->second;
            return true;
        }
    }
    return false;
}

void Navigator::startStudy() {
    ///TODO need to pass study_ to eps
    Epsimulator* eps = new Epsimulator(this);
    eps->showMaximized();
}

void Navigator::systemSettings() {
    SystemDialog* systemDialog = new SystemDialog(this);
    systemDialog->setStudyPath(study_.path());
    if (systemDialog->exec()) {
        study_.setPath(systemDialog->studyPath());
        saveSettings();
    }
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
    saveSettings();
}
