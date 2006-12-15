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

#include "epsim.h"
#include "epsimulator.h"
#include "filtercatalog.h"
#include "options.h"
#include "navigator.h"
#include "patientdialog.h"
#include "study.h"
#include "studyconfigdialog.h"
#include "systemdialog.h"

#include <qaction.h>
#include <qapplication.h>
#include <qbuttongroup.h>
#include <qdatetimeedit.h>
#include <qdir.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlayout.h>
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

///TODO This constant may be better derived from screen realestate.
#define BUTTON_SIZE 70

/**
 * Constructor for TableListViewItem subclass of Navigator
 * @param parent 
 * @param study 
 * @param label1 
 * @param label2 
 * @param label3 
 * @param label4 
 * @param label5 
 * @param label6 
 * @param label7 
 * @param label8 
 */

Navigator::TableListViewItem::TableListViewItem(TableListView* parent, const Study& study,
    QString label1, QString label2, QString label3, 
    QString label4, QString label5, QString label6, 
    QString label7, QString label8 ) 
    : QListViewItem(parent, label1, label2, label3, 
      label4, label5, label6, label7, label8), study_(study) {
}

Navigator::TableListViewItem::~TableListViewItem() {
}

Navigator::TableListView::TableListView(QWidget* parent) 
    : QListView(parent) {
    connect(this, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)), 
        parent->parent(), SLOT(newStudy()));
}

Navigator::TableListView::~TableListView() {
}

/**
 * Loads studies catalog file
 * @param fileName is name of file to be loaded.
 * @return false if catalog file can't be loaded for any reason
 */
bool Navigator::TableListView::load(const QString& fileName) {
    QFile file(fileName);
    // create a studies file if it doesn't exist already
    if (!file.exists()) 
        save(fileName);
    if (!file.open(IO_ReadOnly)) {
        ioError(file, tr("Cannot open file %1 for reading"));
        return false;
    }
    QDataStream in(&file);
    in.setVersion(5);
    Q_UINT32 magic;
    in >> magic;
    if (magic != MagicNumber) {
        error(file, tr("File %1 is not a EP Study file"));
        return false;
    }
    readFromStream(in);
    if (file.status() != IO_Ok) {
        ioError(file, tr("Error reading from file %1"));
        return false;
    }
    return true;
}

bool Navigator::TableListView::save(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(IO_WriteOnly)) {
        ioError(file, tr("Cannot open file %1 for writing"));
        return false;
    }
    QDataStream out(&file);
    out.setVersion(5);
    out << (Q_UINT32)MagicNumber;
    writeToStream(out);
    if (file.status() != IO_Ok) {
        ioError(file, tr("Error writing to file %1"));
        return false;
    }
    return true;
}

void Navigator::TableListView::addStudy(const Study& study) {
    (void) new TableListViewItem(this, study,
            study.isPreregisterStudy() ? tr("Pre-Register") : tr("Study"),
            study.name().fullName(true, true),
            study.mrn(),
            study.dateTime().toString(),
            study.config(),
            study.number(),
            //study.path(),
            ///FIXME below is temporary
            study.dataFile(),
            study.key());
}

void Navigator::TableListView::readFromStream(QDataStream& in) {
    clear();
    while (!in.atEnd()) {
        Study study;
        in >> study;
        addStudy(study);
    }
}

void Navigator::TableListView::writeToStream(QDataStream& out) {
  QListViewItemIterator it(this);
  while (it.current()) {
    QListViewItem* item = *it;
    out << dynamic_cast<TableListViewItem*>(item)->study();
    ++it;
  }
}
    
void Navigator::TableListView::error(const QFile& file, const QString& message) {
    QMessageBox::warning(0, tr("EP Simulator"), message.arg(file.name()));
}

void Navigator::TableListView::ioError(const QFile& file, const QString& message) {
    error(file, message + ": " + file.errorString());
}

/**
 * Navigator constructor
 */

Navigator::Navigator(QWidget* parent, const char* name)
 : QMainWindow( parent, name, WDestructiveClose ) {
    options_ = Options::instance();

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

    sourceLabel_ = new QLabel(tr(" Source: %1 ").arg(options_->studyPath()), this);
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
    button->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    button->setPixmap(QPixmap::fromMimeSource(pixmapName));
    static int row = 0;   // allows adding widgets in correct row
    // last parameter centers the buttons and labels horizontally
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

void Navigator::createCentralWidget() {
    horizontalSplitter = new QSplitter(Horizontal, this);
    setCentralWidget(horizontalSplitter);

    buttonFrame = new QFrame(horizontalSplitter);
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

    tableListView_ = new TableListView(horizontalSplitter);
    tableListView_->addColumn(tr("Study Type"));         // col 0
    tableListView_->addColumn(tr("Patient"));            // col 1
    tableListView_->addColumn(tr("MRN"));                // col 2
    tableListView_->addColumn(tr("Study Date/Time"));    // col 3
    tableListView_->addColumn(tr("Study Config"));       // col 4
    tableListView_->addColumn(tr("Study Number"));       // col 5
    tableListView_->addColumn(tr("Location of Study"));  // col 6
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
    readSettings(); 
    refreshCatalog();
}

void Navigator::refreshCatalog() {
    tableListView_->load(options_->studyPath() + "studies.eps");
}

void Navigator::regenerateCatalog() {
    ///TODO this will read through the studies in the directory and recreate the catalog
}


void Navigator::saveSettings() {
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");

    QString str;
    QTextOStream out(&str);
    out << *horizontalSplitter;
    settings.writeEntry("/horizontalSplitter", str);
    settings.endGroup();
}

void Navigator::readSettings() {
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");
    
    QString str = settings.readEntry("/horizontalSplitter");
    QTextIStream in(&str);
    in >> *horizontalSplitter;
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
    setupAction(exportAct_, "Export study", 0);
    exitAct = new QAction(tr("E&xit"), tr("Ctrl+Q"), this);
    setupAction(exitAct, "Exit EP Simulator", SLOT(close()));

    // Catalog menu
    switchAct_ = new QAction(tr("Switch"), 0, this);
    setupAction(switchAct_, "Switch", 0);
    filterStudiesAct_ = new QAction(tr("Filter Studies..."), 0, this);
    setupAction(filterStudiesAct_, "Filter studies", SLOT(filterStudies()), "hi32-filterstudies.png");
    removeStudiesFilterAct_ = new QAction(tr("Remove Studies Filter"), 0, this);
    setupAction(removeStudiesFilterAct_, "Remove studies filter",
	SLOT(unfilterStudies()), "hi32-removefilter.png");
    // inactivate removeStudiesFilterAct_ by default
    removeStudiesFilterAct_->setEnabled(false);
    refreshViewAct_ = new QAction(tr("Refresh"), 0, this);
    setupAction(refreshViewAct_, "Refresh the catalog", SLOT(refreshCatalog()), "hi32-refreshcatalog.png");
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
    filterStudiesAct_->addTo(navigatorToolBar_);
    removeStudiesFilterAct_->addTo(navigatorToolBar_);
    navigatorToolBar_->addSeparator();
    refreshViewAct_->addTo(navigatorToolBar_);
     
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
    switchAct_->addTo(catalogMenu);
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
        study_.setPath(options_->studyPath());
        tableListView_->addStudy(study_);
        return true;
    }
    return false;
}

void Navigator::filterStudies() {
    FilterCatalog* filterCatalog = new FilterCatalog(this);
    if (filterCatalog->exec()) {
	QRegExp lastNameRegEx(filterCatalog->lastNameLineEdit_->text(),
			      false, true);
	QRegExp firstNameRegEx(filterCatalog->firstNameLineEdit_->text(),
			      false, true);
	QRegExp mrnRegEx(filterCatalog->mrnLineEdit_->text(),
			      false, true);
	QRegExp studyConfigRegEx(filterCatalog->studyConfigLineEdit_->text(),
			      false, true);
	QRegExp studyNumberRegEx(filterCatalog->studyNumberLineEdit_->text(),
			      false, true);
	QRegExp studyFileRegEx(filterCatalog->studyFileLineEdit_->text(),
			      false, true);
	// date stuff next
	QDate today = QDate::currentDate();
	QDate startDate = today, endDate = today;
	bool anyDate = false;
	switch (filterCatalog->studyDateButtonGroup_->selectedId()) {
	    case 0 : 
		anyDate = true;
		break;

	    case 1 : // today, default settings are true
		break;

	    case 2 : 
		startDate = endDate.addDays(-7);
		break; // i.e. last week's studies

	    case 3 : 
		startDate = filterCatalog->beginDateEdit_->date();
		endDate = filterCatalog->endDateEdit_->date();
		break;
	}	       
	
	filterLabel_->setText(tr(" Filtered "));
	statusBar()->update();	
        filterStudiesAct_->setEnabled(false);
        removeStudiesFilterAct_->setEnabled(true);
	// rest of processing here   
    }
}

void Navigator::unfilterStudies() {
    // do the unfiltering here
    filterLabel_->setText(tr(" Unfiltered "));
    statusBar()->update();
    removeStudiesFilterAct_->setEnabled(false);
    filterStudiesAct_->setEnabled(true);
}

void Navigator::newStudy() {
///TODO study_ must be "blank" unless a study is selected in the catalog.
/// Same thing for preregister and continue study
    if (!studySelected()) {
        ///TODO is this really the best way to do this???
        Study newStudy;
        study_ = newStudy;
    }
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
        study_ = static_cast<TableListViewItem*>(item)->study();
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
    systemDialog->setStudyPath(options_->studyPath());
    if (systemDialog->exec()) {
        options_->setStudyPath(systemDialog->studyPath());
        options_->writeSettings();
        // status bar and catalog might be changed 
        sourceLabel_->setText(tr(" Source: %1 ").arg(options_->studyPath()));
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
    tableListView_->save(options_->studyPath() + "studies.eps");
    delete options_;
}
