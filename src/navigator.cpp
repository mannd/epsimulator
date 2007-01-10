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

/** \file
    Basically the navigator window is the main window.  When switching to
    the epsimulator window, we will actually just be changing the menus
    and the central widget
*/

#include "catalog.h"
#include "catalogcombobox.h"
#include "epsim.h"
#include "epsimulator.h"
#include "filtercatalog.h"
#include "options.h"
#include "navigator.h"
#include "patientdialog.h"
#include "study.h"
#include "studyconfigdialog.h"
#include "systemdialog.h"
#include "tablelistview.h"

#include <qaction.h>
#include <qapplication.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qdatetimeedit.h>
#include <qdir.h>
#include <qframe.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include <qsettings.h>
#include <qsizepolicy.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qtoolbar.h>

#include <algorithm>
#include <stdlib.h>

#ifndef NDEBUG
#include <iostream> // for debugging
#endif

/**
 * Navigator constructor
 */

Navigator::Navigator(QWidget* parent, const char* name)
 : QMainWindow( parent, name, WDestructiveClose ) ,
   options_(Options::instance()), catalogs_(0) {
    // omnipresent, holding last filter
    filterCatalog_ = new FilterCatalog(this);   

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createCentralWidget();

    setCaption(tr("%1 Navigator").arg(PROGRAM_NAME));
    setIcon(QPixmap::fromMimeSource("hi32-app-epsimulator.png"));
}

void Navigator::createStatusBar() {
    messageLabel_ = new QLabel(tr("For Help, press F1"), this);

    ///TODO make sure getenv works for Windows too
    userLabel_ = new QLabel(tr(" User: %1 ").arg(std::getenv("USER")), this);
    userLabel_->setAlignment(AlignHCenter);
    userLabel_->setMinimumSize(userLabel_->sizeHint());

    sourceLabel_ = new QLabel(tr(" Source: %1 ").arg(systemPath()), this);
    sourceLabel_->setAlignment(AlignHCenter);
    sourceLabel_->setMinimumSize(sourceLabel_->sizeHint());

    filterLabel_ = new QLabel(tr(" Unfiltered "), this);
    filterLabel_->setAlignment(AlignHCenter);
    filterLabel_->setMinimumSize(filterLabel_->sizeHint());

    statusBar()->addWidget(messageLabel_, 1);
    statusBar()->addWidget(userLabel_);
    statusBar()->addWidget(sourceLabel_);
    statusBar()->addWidget(filterLabel_);
}

/**
 * Sets up each square button along the side of the Navigator window.
 * @param frame 
 * @param  
 */
void Navigator::setupButton(QPushButton* button, QString pixmapName,
                             QLabel* label, const char* slotName, bool lastButton) {
    button->setFixedSize(buttonSize, buttonSize);
    button->setPixmap(QPixmap::fromMimeSource(pixmapName));
    static int row = 0;   // allows adding widgets in correct row
    // last parameter centers the buttons and labels horizontally
    if (row == 0) {
        // insert blank row at top -- looks better with this!
        QLabel* topLabel = new QLabel("", buttonFrame);  
        topLabel->setAlignment(int(QLabel::AlignCenter));
        buttonFrameLayout->addWidget(topLabel, row++, 0);
    }
    buttonFrameLayout->addWidget(button, row++, 0, Qt::AlignHCenter);
    // Notice that a SLOT is passed as a function parameter as a const char*.
    if (slotName)
        connect(button, SIGNAL(clicked()), this, slotName); 
    label->setPaletteForegroundColor("white");
    label->setAlignment(int(QLabel::AlignCenter));
    buttonFrameLayout->addWidget(label, row++, 0, Qt::AlignHCenter);
    QLabel* spaceLabel = new QLabel("", buttonFrame);   // insert line between button/label groups
    spaceLabel->setAlignment(int(QLabel::AlignCenter));
    buttonFrameLayout->addWidget(spaceLabel, row++, 0);
    if (lastButton) {
        spacer = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
        buttonFrameLayout->addItem( spacer, row, 0 );
    }
}

void Navigator::createButtonFrame() {
    horizontalSplitter_ = new QSplitter(Horizontal, this);
    setCentralWidget(horizontalSplitter_);

    buttonFrame = new QFrame(horizontalSplitter_);
    buttonFrame->setSizePolicy(QSizePolicy( (QSizePolicy::SizeType)1, 
                              (QSizePolicy::SizeType)5, 0, 0,
                               buttonFrame->sizePolicy().hasHeightForWidth() ) );
    buttonFrame->setFrameShape(QFrame::StyledPanel);
    buttonFrame->setPaletteBackgroundColor("blue");
    buttonFrame->setMaximumWidth(200);
    buttonFrameLayout = new QGridLayout(buttonFrame, 1, 1, 11, 6, "");

    newStudyButton = new QPushButton(buttonFrame);
    newStudyLabel = new QLabel(tr("New Study"), buttonFrame);
    setupButton(newStudyButton, "hi64-newstudy.png", newStudyLabel, SLOT(newStudy()));

    continueStudyButton = new QPushButton(buttonFrame);
    continueStudyLabel = new QLabel(tr("Continue Study"), buttonFrame);
    setupButton(continueStudyButton, "hi64-continuestudy.png", continueStudyLabel, 0 /* slot */);

    
    reviewStudyButton = new QPushButton(buttonFrame);
    reviewStudyLabel = new QLabel(tr("Review Study"), buttonFrame);
    setupButton(reviewStudyButton, "hi64-reviewstudy.png", reviewStudyLabel, 0 /* slot */);

    preregisterPatientButton = new QPushButton(buttonFrame);
    preregisterPatientLabel = new QLabel(tr("Pre-Register"), buttonFrame);
    setupButton(preregisterPatientButton, "hi64-preregister.png", preregisterPatientLabel, 
        SLOT(preregisterPatient()));

    reportsButton = new QPushButton(buttonFrame);
    reportsLabel = new QLabel(tr("Reports"), buttonFrame);
    setupButton(reportsButton, "hi64-reports.png", reportsLabel, 0 /* slot */, true);

}

void Navigator::createTableListView() {
    tableListView_ = new TableListView(horizontalSplitter_, options_);
    tableListView_->addColumn(tr("Study Type"));         // col 0
    tableListView_->addColumn(tr("Patient"));            // col 1
    tableListView_->addColumn(tr("MRN"));                // col 2
    tableListView_->addColumn(tr("Study Date/Time"));    // col 3
    tableListView_->addColumn(tr("Study Config"));       // col 4
    tableListView_->addColumn(tr("Study Number"));       // col 5
    tableListView_->addColumn(tr("Location of Study"));  // col 6
    /// FIXME Below can be eliminated, but must also eliminate keycolumn
    tableListView_->addColumn(tr("Hidden key"));         // col 7

    tableListView_->setAllColumnsShowFocus(true);
    tableListView_->setShowSortIndicator(true);
    tableListView_->setSortColumn(3);    // default sort is date/time
    // hide the hidden key column: make sure it is defined correctly in keyColumn
    int keyColumn = 7;
    tableListView_->setColumnWidthMode(keyColumn, QListView::Manual);
    tableListView_->hideColumn(keyColumn);
    tableListView_->header()->setResizeEnabled(false, keyColumn);
    //tableListView_->setResizeMode(QListView::AllColumns);
    // above messes up the hidden column
    connect(catalogComboBox_, SIGNAL(activated(int)),
        this, SLOT(changeCatalog()));


}

void Navigator::createCentralWidget() {
    createButtonFrame();
    createTableListView();
    readSettings(); 
    refreshCatalog();
}

void Navigator::refreshCatalog() {
    catalogComboBox_->refresh();
    /// TODO below is correct!!!!
//    tableListView_->load(catalogs_->currentCatalog()->path());
    tableListView_->load(systemPath() + "/studies.eps");
    // reapply filter if present
    if (tableListView_->filtered())
        processFilter();
}

void Navigator::regenerateCatalog() {
    ///TODO this will read through the studies in the directory and recreate the catalog
    /// filter has to be cleared for this to work.
}

void Navigator::changeCatalog() {
    switch (catalogComboBox_->source()) {
        case Network:
            /// TODO load network catalog
            break;
        case System:
            /// TODO load system catalog
            break;
        case Optical:
            /// TODO load optical catalog
            break;
        case Other:
            /// TODO load other catalog
            break;
        default:
            // is always there, a safe default
            break;
    }
}


//  void Navigator::changeCatalog() {
//     tableListView_->setCatalog(catalogComboBox_->currentItem());
// }

// void Navigator::setCatalog(CatalogSource source) {
// }

// void Navigator::changeCatalog(CatalogSource source) {
//     catalogComboBox_->setSource(source);
//     // tableListView_->setSource(source);
// }

void Navigator::setCatalogNetwork() {
    catalogComboBox_->setSource(Network);
    changeCatalog();
}

void Navigator::setCatalogSystem() {
    catalogComboBox_->setSource(System);
    changeCatalog();
}

void Navigator::setCatalogOptical() {
    catalogComboBox_->setSource(Optical);
    changeCatalog();
}

void Navigator::setCatalogOther() {
    catalogComboBox_->setSource(Other);
    changeCatalog();
}

void Navigator::saveSettings() {
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");

    QString str;
    QTextOStream out(&str);
    out << *horizontalSplitter_;
    settings.writeEntry("/horizontalSplitter", str);
    settings.endGroup();
}

void Navigator::readSettings() {
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");
    
    QString str = settings.readEntry("/horizontalSplitter");
    QTextIStream in(&str);
    in >> *horizontalSplitter_;
    settings.endGroup();
}


/**
 * Sets up icon, status tip, and slot for an action.
 */
void Navigator::setupAction(QAction* action, QString statusTip, 
                            const char* slotName, const char* iconName) {
    if (iconName)
        action->setIconSet(QPixmap::fromMimeSource(iconName));
    action->setStatusTip(tr(statusTip));
    if (slotName)
        connect(action, SIGNAL(activated()), this, slotName);
} 


void Navigator::createActions() {
    // Study menu
    newAct = new QAction(tr("&New..."), tr("Ctrl+N"), this);
    setupAction(newAct, "New study", SLOT(newStudy()), "hi32-newstudy.png");
    continueAct = new QAction(tr("&Continue"), 0, this);
    setupAction(continueAct, "Continue study", 0, "hi32-continuestudy.png");
    reviewAct = new QAction(tr("&Review"), 0, this);
    setupAction(reviewAct, "Review study", 0, "hi32-reviewstudy.png");
    preregisterAct = new QAction(tr("&Pre-Register"), 0, this);
    setupAction(preregisterAct, "Pre-register patient", 
        SLOT(preregisterPatient()), "hi32-preregister.png");
    reportsAct = new QAction(tr("R&eports..."), 0, this);
    setupAction(reportsAct, "Procedure reports", 0, "hi32-reports.png" );
    copyAct = new QAction(tr("Copy..."), 0, this);
    setupAction(copyAct, "Copy study", 0);
    deleteAct = new QAction(tr("Delete..."), 0, this);
    setupAction(deleteAct, "Delete study", SLOT(deleteStudy()));
    exportAct_ = new QAction(tr("Export..."), 0, this);
    setupAction(exportAct_, "Export study", 0, "hi32-exportstudy.png");
    exitAct = new QAction(tr("E&xit"), tr("Ctrl+Q"), this);
    setupAction(exitAct, "Exit EP Simulator", SLOT(close()));

    // Catalog menu
//    switchAct_ = new QAction(tr("Switch..."), 0, this);
//    setupAction(switchAct_, "Switch catalogs", 0);
    // Submenu of Switch...
    // an action "Achive Server" is skipped here, but is present on Prucka
    networkSwitchAct_ = new QAction(tr("Network"), 0, this);
    setupAction(networkSwitchAct_, "Switch to network catalog", 
        SLOT(setCatalogNetwork()));
    // networkSwitchAct_ only enabled if set in options
    networkSwitchAct_->setEnabled(options_->enableNetworkStorage());
    systemSwitchAct_ = new QAction(tr("System"), 0, this);
    setupAction(systemSwitchAct_, "Switch to system catalog", 
        SLOT(setCatalogSystem()));
    opticalSwitchAct_ = new QAction(tr("Optical"), 0, this);
    setupAction(opticalSwitchAct_, "Switch to optical catalog", 
        SLOT(setCatalogOptical()));
    // back to main menu items
    browseSwitchAct_ = new QAction(tr("Browse..."), 0, this);
    setupAction(browseSwitchAct_, "Browse for catalog files", 
        SLOT(setCatalogOther()));
    filterStudiesAct_ = new QAction(tr("Filter Studies..."), 0, this);
    setupAction(filterStudiesAct_, "Filter studies", 
        SLOT(filterStudies()), "hi32-filterstudies.png");
    removeStudiesFilterAct_ = new QAction(tr("Remove Studies Filter"), 0, this);
    setupAction(removeStudiesFilterAct_, "Remove studies filter",
	SLOT(unfilterStudies()), "hi32-removefilter.png");
    // inactivate removeStudiesFilterAct_ by default
    removeStudiesFilterAct_->setEnabled(false);
    refreshViewAct_ = new QAction(tr("Refresh"), 0, this);
    setupAction(refreshViewAct_, "Refresh the catalog", 
        SLOT(refreshCatalog()), "hi32-refreshcatalog.png");
    regenerateAct_ = new QAction(tr("Regenerate"), 0, this);
    setupAction(regenerateAct_, "Regenerate the catalog",
	SLOT(regenerateCatalog()));
    relabelDiskAct_ = new QAction(tr("Re-Label Disk..."), 0 ,this);
    setupAction(relabelDiskAct_, "Re-label the optical disk", 0);
    mergeStudiesAct_ = new QAction(tr("Merge Studies..."), 0, this);
    setupAction(mergeStudiesAct_, "Merge studies together", 0);

    // Utilities menu
    exportListsAct_ = new QAction(tr("Export Lists..."), 0, this);
    setupAction(exportListsAct_, "Export lists", 0);
    exportReportFormatsAct_ = new QAction(tr("Export Report Formats..."), 0, this);
    setupAction(exportReportFormatsAct_, "Export report formats", 0);
    importListsAct_ = new QAction(tr("Import Lists..."), 0, this);
    setupAction(importListsAct_, "Import lists", 0);
    importReportFormatsAct_ = new QAction(tr("Import Report Formats..."), 0, this);
    setupAction(importReportFormatsAct_, "Import report formats", 0);
    ejectOpticalDiskAct_ = new QAction(tr("Eject Optical Disk"), 0, this);
    setupAction(ejectOpticalDiskAct_, "Eject optical disk", 0);

    // Administration menu
    loginAct = new QAction(tr("Login..."), 0, this);
    setupAction(loginAct, "Login", 0);
    logoutAct = new QAction(tr("Logout"), 0, this);
    setupAction(logoutAct, "Logout", 0);
    changePasswordAct = new QAction(tr("Change Password..."), 0, this);
    setupAction(changePasswordAct, "Change administrator password", 0);
    intervalsAct = new QAction(tr("Intervals"), 0, this);
    setupAction(intervalsAct, "Intervals", 0);
    columnFormatsAct = new QAction(tr("Column Formats"), 0, this);
    setupAction(columnFormatsAct, "Column formats", 0);
    protocolsAct = new QAction(tr("Protocols"), 0, this);
    setupAction(protocolsAct, "Protocols", 0);
    studyConfigurationsAct = new QAction(tr("Study Configurations"), 0, this);
    setupAction(studyConfigurationsAct, "Study configurations", 0);
    systemSettingsAct = new QAction(tr("System Settings"), 0, this);
    setupAction(systemSettingsAct, "Change system settings", SLOT(systemSettings()));

    // Help menu
    epsimulatorHelpAct_ = new QAction(tr("EP Simulator Help..."), tr("F1"), this);
    setupAction(epsimulatorHelpAct_, "Get help for EP Simulator", SLOT(help()));
    aboutAct = new QAction(tr("&About EP Simulator"), 0, this);
    setupAction(aboutAct, "About EP Simulator", SLOT(about()));
}

void Navigator::createToolBars() {
    navigatorToolBar_ = new QToolBar(tr("Navigator"), this);
    catalogComboBox_ = new CatalogComboBox(navigatorToolBar_, "catalogComboBox");
    navigatorToolBar_->addSeparator();
    filterStudiesAct_->addTo(navigatorToolBar_);
    removeStudiesFilterAct_->addTo(navigatorToolBar_);
    navigatorToolBar_->addSeparator();
    refreshViewAct_->addTo(navigatorToolBar_);
    navigatorToolBar_->addSeparator();
    exportAct_->addTo(navigatorToolBar_);
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
    exportAct_->addTo(studyMenu);
    studyMenu->insertSeparator();
    exitAct->addTo(studyMenu);

    catalogMenu = new QPopupMenu(this);
    switchSubMenu_ = new QPopupMenu(this);
 //   switchAct_->addTo(catalogMenu);
    networkSwitchAct_->addTo(switchSubMenu_);
    systemSwitchAct_->addTo(switchSubMenu_);
    opticalSwitchAct_->addTo(switchSubMenu_);
    browseSwitchAct_->addTo(switchSubMenu_);
    catalogMenu->insertItem(tr("Switch"), switchSubMenu_);
    filterStudiesAct_->addTo(catalogMenu);
    removeStudiesFilterAct_->addTo(catalogMenu);
    catalogMenu->insertSeparator();
    refreshViewAct_->addTo(catalogMenu);
    regenerateAct_->addTo(catalogMenu);
    relabelDiskAct_->addTo(catalogMenu);
    mergeStudiesAct_->addTo(catalogMenu);

    utilitiesMenu = new QPopupMenu(this);
    exportListsAct_->addTo(utilitiesMenu);
    exportReportFormatsAct_->addTo(utilitiesMenu);
    utilitiesMenu->insertSeparator();
    importListsAct_->addTo(utilitiesMenu);
    importReportFormatsAct_->addTo(utilitiesMenu);
    utilitiesMenu->insertSeparator();
    ejectOpticalDiskAct_->addTo(utilitiesMenu);

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
    epsimulatorHelpAct_->addTo(helpMenu);
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
        /// FIXME this depends on the catalogComboBox
        study_.setPath(options_->opticalStudyPath());
        study_.setFile(study_.fileName());
        tableListView_->addStudy(study_);
        // write the study to the catalog now in case user decides to refresh later
        tableListView_->save(options_->systemCatalogPath() + "/studies.eps");
        return true;
    }
    return false;
}

void Navigator::processFilter() {
        // if the LineEdit is blank, it matches anything, so make it "*"
       	QRegExp lastNameRegExp(filterCatalog_->lastNameLineEdit->text().isEmpty()
            ? "*" : filterCatalog_->lastNameLineEdit->text(), false, true);
	QRegExp firstNameRegExp(filterCatalog_->firstNameLineEdit->text().isEmpty()
            ? "*" : filterCatalog_->firstNameLineEdit->text(), false, true);
	QRegExp mrnRegExp(filterCatalog_->mrnLineEdit->text().isEmpty()
            ? "*" : filterCatalog_->mrnLineEdit->text(), false, true);
	QRegExp studyConfigRegExp(
            filterCatalog_->studyConfigLineEdit->text().isEmpty()
            ? "*" :filterCatalog_->studyConfigLineEdit->text(), false, true);
	QRegExp studyNumberRegExp(
            filterCatalog_->studyNumberLineEdit->text().isEmpty()
            ? "*" : filterCatalog_->studyNumberLineEdit->text(), false, true);
	QRegExp studyFileRegExp(filterCatalog_->studyFileLineEdit->text().isEmpty()
            ? "*" : filterCatalog_->studyFileLineEdit->text(), false, true);
	// date stuff next
	QDate today = QDate::currentDate();
	QDate startDate = today, endDate = today;
	bool anyDate = false;
	switch (filterCatalog_->studyDateButtonGroup->selectedId()) {
	    case 0 : 
		anyDate = true;
		break;

	    case 1 : // today, default settings are true
		break;

	    case 2 : 
		startDate = endDate.addDays(-7);
		break; // i.e. last week's studies

	    case 3 :   // specific dates selected
		startDate = filterCatalog_->beginDateEdit->date();
		endDate = filterCatalog_->endDateEdit->date();
		break;
	}	
        FilterStudyType filterStudyType = static_cast<FilterStudyType>(
            filterCatalog_->studyTypeComboBox->currentItem());
        tableListView_->applyFilter(filterStudyType, lastNameRegExp,
            firstNameRegExp, mrnRegExp, studyConfigRegExp, 
            studyNumberRegExp, studyFileRegExp, 
            anyDate, startDate, endDate);
	filterLabel_->setText(tr(" Filtered "));
	statusBar()->update();	
        filterStudiesAct_->setEnabled(false);
        removeStudiesFilterAct_->setEnabled(true);
        // also disallow refreshing while filtered
//        refreshViewAct_->setEnabled(false);
// only disallow regenerating when catalog is filtered
        regenerateAct_->setEnabled(false);
}

void Navigator::filterStudies() {
//    FilterCatalog* filterCatalog = new FilterCatalog(this);
    if (filterCatalog_->exec()) {
        processFilter();
    }
}

void Navigator::unfilterStudies() {
    // do the unfiltering here
    tableListView_->removeFilter();
    filterLabel_->setText(tr(" Unfiltered "));
    statusBar()->update();
    removeStudiesFilterAct_->setEnabled(false);
    filterStudiesAct_->setEnabled(true);
//    refreshViewAct_->setEnabled(true);
    regenerateAct_->setEnabled(true);
}

void Navigator::prepareStudy() {
    if (!studySelected()) {
        Study newStudy;
        study_ = newStudy;
    }
}

void Navigator::newStudy() {
///TODO study_ must be "blank" unless a study is selected in the catalog.
/// Same thing for preregister and continue study
    prepareStudy();
    StudyConfigDialog* studyConfigDialog  = new StudyConfigDialog(this);
    if (studyConfigDialog->exec()) {
///TODO StudyConfigDialog should probably be SelectConfigDialog and 
/// need to fix below
        study_.setConfig(studyConfigDialog->configListBox->currentText());
        if (getStudyInformation()) {
            startStudy();
        }
    }
}

void Navigator::preregisterPatient() {
    prepareStudy();
    study_.setConfig("");   // preregistered study has no config info
    getStudyInformation();
}

void Navigator::deleteStudy() {
    if (QListViewItem* item = tableListView_->selectedItem()) {
        int ret = QMessageBox::warning(
            this, tr("Delete Study?"),
            tr("The selected study will be permanently "
               "deleted.  Do you wish to continue?"),
            QMessageBox::Yes ,
            QMessageBox::No | QMessageBox::Default, // default is NO!
            QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes) {
            delete item;
            deleteDataFile();
        }
    } 
    else {
        QMessageBox::information(this, tr("No Study Selected"),
            tr("You must first select a study to delete it."),
            QMessageBox::Ok);
    }
}

/// returns true and switches to study highlighted in the catalog; otherwise returns /// false if no study highlighted and leaves study_ unchanged
bool Navigator::studySelected() {
    if (QListViewItem* item = tableListView_->selectedItem()) {
        // must cast item to a TableListViewItem* to get study from it.
        study_ = dynamic_cast<TableListViewItem*>(item)->study();
        return true;
    }
    return false;
}

void Navigator::deleteDataFile() {
}

void Navigator::startStudy() {
    ///TODO need to pass study_ to eps
    Epsimulator* eps = new Epsimulator(this);
    eps->showMaximized();
}

///TODO The studies.eps file cannot change with the studyPath, as it must contain the different 
/// paths for all the studies in the catalog.  studies.eps must be in a fixed place, probably a
/// relative path to the executable, or in the executable directory, e.g.  Must fix this soon.
void Navigator::systemSettings() {
    SystemDialog* systemDialog = new SystemDialog(this);
    systemDialog->opticalStudyPathLineEdit->setText(options_->opticalStudyPath());
    systemDialog->networkStudyPathLineEdit->setText(options_->networkStudyPath());
    systemDialog->exportFilePathLineEdit->setText(options_->exportFilePath());
    systemDialog->enableAcquisitionCheckBox->setChecked(options_->enableAcquisition());
    systemDialog->emulateOpticalDriveCheckBox->setChecked(options_->emulateOpticalDrive());
    systemDialog->setEnableFileExportCheckBox(options_->enableFileExport());
    systemDialog->setEnableNetworkStorageCheckBox(options_->enableNetworkStorage());
    if (systemDialog->exec()) {
        /// FIXME These can just be data members, e.g. opticalStudyLineEdit->text()
        options_->setOpticalStudyPath(systemDialog->opticalStudyPathLineEdit->text());
        options_->setNetworkStudyPath(systemDialog->networkStudyPathLineEdit->text());
        options_->setExportFilePath(systemDialog->exportFilePathLineEdit->text());
        options_->setEnableAcquisition(systemDialog->enableAcquisitionCheckBox->isChecked());
        options_->setEmulateOpticalDrive(systemDialog->emulateOpticalDriveCheckBox->isChecked());
        options_->setEnableFileExport(systemDialog->enableFileExportCheckBox->isChecked());
        options_->setEnableNetworkStorage(systemDialog->enableNetworkStorageCheckBox->isChecked()); 
        options_->writeSettings();
        // menu is changed
        networkSwitchAct_->setEnabled(options_->enableNetworkStorage());
        // status bar and catalog might be changed 
        /// FIXME below depends on catalogComboBox
        sourceLabel_->setText(tr(" Source: %1 ").arg(options_->systemCatalogPath()));
        sourceLabel_->setMinimumSize(sourceLabel_->sizeHint());
        statusBar()->update();
        refreshCatalog();
    }
}

void Navigator::help() {
    QMessageBox::information(this, tr("%1 Help").arg(PROGRAM_NAME),
        tr("Help is available from www.epstudiossoftware.com"),
        QMessageBox::Ok);
}

void Navigator::about() {
    Epsimulator::about(this);
}

void Navigator::closeEvent(QCloseEvent *event) {
    int ret = QMessageBox::question(
            this,
            tr("Really quit?"),
            tr("Quit %1").arg(PROGRAM_NAME),
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
    tableListView_->save(systemPath() + "/studies.eps");
    delete catalogs_;
}
