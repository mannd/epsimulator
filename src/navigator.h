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

/** 
 *  \file header file for the Navigator classs,
 *  which is the main window for the program.
 */

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "epsim.h"  // for CatalogSource
#include "options.h"
#include "study.h"
//#include "catalog.h"

#include <qdatetime.h>
#include <qdir.h>
#include <qfile.h>
#include <qlistview.h>
#include <qmainwindow.h>
#include <qregexp.h>

class CatalogComboBox;
class FilterCatalog;
class Options;

class QAction;
class QFrame;
class QGridLayout;
class QLabel;
class QPopupMenu;
class QPushButton;
class QSpacerItem;
class QSplitter;
class QToolBar;

/**
	@author David Mann <mannd@epstudiossoftware.com>
*/

using namespace epsim;  /// FIXME for now has CatalogSource enum

class Navigator : public QMainWindow {
    Q_OBJECT

public:
    Navigator(QWidget* parent = 0, const char *name = "navigator");
    ~Navigator();
  
protected:
    void closeEvent(QCloseEvent * event);
    //    void contextMenuEvQDir::current().path() + "/system"ent(QContextMenuEvent * event);

private slots:
    void newStudy();
    void preregisterPatient();
    void deleteStudy();
    void filterStudies();
    void unfilterStudies();
    void refreshCatalog();        

    void regenerateCatalog();
    
    // these must be separate due to Qt Signal/Slot mechanism.  Can't pass
    // a parameter to a common slot.
    void setCatalogNetwork();
    void setCatalogSystem();
    void setCatalogOptical();
    void setCatalogOther();

    void systemSettings();

    void help();
    void about();

private:

    static const int buttonSize = 70;   // size of square buttons in blue panel

    enum FilterStudyType {AnyStudyType, StudyType, PreregisterType};


    class TableListView : public QListView {
//        friend class Navigator; // allows use of Navigator data members

    public:

        TableListView(QWidget* parent, Options* options);
        ~TableListView();

        bool filtered() const {return filtered_;}

        bool load(const QString& fileName);
        bool save(const QString& fileName);
        void addStudy(const Study& study);
        void applyFilter(FilterStudyType filterStudyType,
                         QRegExp lastName,
                         QRegExp firstName,
                         QRegExp mrn,
                         QRegExp studyConfig,
                         QRegExp studyNumber,
                         QRegExp studyFile,
                         bool anyDate,
                         const QDate& startDate,
                         const QDate& endDate);
/*        void applyFilter();*/
        void removeFilter();
        void showTable();    // shows table, depending on catalogSource and filter
//        void setCatalog(int catalogComboBoxSelectedId);
    
    public slots:
        void changeCatalog(int);

    private:
        enum {MagicNumber = 0x99c798f2};    // first bytes of EP Simulator binary files

        void readFromStream(QDataStream& in);
        void writeToStream(QDataStream& out);
        void error(const QFile& file, const QString& message);
        void ioError(const QFile& file, const QString& message);
        
        bool filtered_;
        Options* options_;  // copy of Navigator options_
        // for dependency, options_ must be declared before catalogSource_
        CatalogSource catalogSource_;
    }; // TableListView

    class TableListViewItem : public QListViewItem {

    public:
        TableListViewItem(TableListView* parent, const Study& study,
                          QString label1, 
                          QString label2 = QString::null, 
                          QString label3 = QString::null, 
                          QString label4 = QString::null, 
                          QString label5 = QString::null, 
                          QString label6 = QString::null, 
                          QString label7 = QString::null, 
                          QString label8 = QString::null );
        ~TableListViewItem();

        void setFilteredOut(bool filteredOut) {filteredOut_ = filteredOut;}

        Study study() {return study_;}
        bool filteredOut() const {return filteredOut_;}

    private:
        Study study_;
        bool filteredOut_;
 //       CatalogSource catalogSource_;
    }; // TableListViewItem

    // Functions to set up the Navigator main window.
    void createButtonFrame();
    void createTableListView();
    void createCentralWidget();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void setupButton(QPushButton* button, QString pixmapName, 
                      QLabel* label, const char* slotName, bool lastButton = false);
    void setupAction(QAction* action, QString statusTip,
                     const char* slotName, const char* iconName = 0);

    void saveSettings();
    void readSettings();

    // Filtering
    void processFilter();

    // Study related
    void startStudy();
    bool getStudyInformation();
    bool studySelected();
    void prepareStudy();    // clears study_ if no study selected
    void deleteDataFile();      // delete data file associated with current study
    void createDataFile();      // create a data file for a new study
    void openDataFile();        // open a previous data file for review or appending data

    // Deprecated?
//    void setCatalog(CatalogSource);
//    QString currentCatalogPath();   // holds path of currently displayed catalog
    QString systemPath() {return QDir::homeDirPath();}
    // These are the paths to the 3 catalogs maintained, plus an optional path to 
    // another catalog you can browse to. 
//     QString networkPath_;
//     QString systemPath_;
//     QString opticalPath_;
//     QString otherPath_; 
//     bool emulateOpticalDrive_;  // emulate optical drive on hard drive using subdirectories
//    QString studiesPath_;   // path to studies

    // Data members
    Study study_;           // current study
    Options* options_;
    FilterCatalog* filterCatalog_;  // FilterCatalog Dialog box

    // StatusBar labels
    QLabel* messageLabel_;
    QLabel* userLabel_;
    QLabel* sourceLabel_;
    QLabel* filterLabel_;

    // central widget stuff
    QSplitter* horizontalSplitter_;
    QFrame* buttonFrame;
    QGridLayout* buttonFrameLayout;
    TableListView* tableListView_;
    QPushButton* newStudyButton;
    QPushButton* continueStudyButton;
    QPushButton* reviewStudyButton;
    QPushButton* preregisterPatientButton;
    QPushButton* reportsButton; 
    QLabel* newStudyLabel;
    QLabel* continueStudyLabel;
    QLabel* reviewStudyLabel;
    QLabel* preregisterPatientLabel;
    QLabel* reportsLabel;
    QSpacerItem* spacer;

    // Study menu
    QAction *newAct;
    QAction *continueAct;
    QAction *reviewAct;
    QAction *preregisterAct;
    QAction *reportsAct;
    QAction *copyAct;
    QAction *deleteAct;
    QAction* exportAct_;
    QAction *exitAct;

    // Catalog menu
//    QAction* switchAct_;
    QAction* networkSwitchAct_;
    QAction* systemSwitchAct_;
    QAction* opticalSwitchAct_;
    QAction* browseSwitchAct_;
    QAction* filterStudiesAct_;
    QAction* removeStudiesFilterAct_;
    QAction* refreshViewAct_;
    QAction* regenerateAct_;
    QAction* relabelDiskAct_;
    QAction* mergeStudiesAct_;

    // Utilities menu
    QAction* exportListsAct_;
    QAction* exportReportFormatsAct_;
    QAction* importListsAct_;
    QAction* importReportFormatsAct_;
    QAction* ejectOpticalDiskAct_;

    // Administration menu
    QAction *loginAct;
    QAction *logoutAct;
    QAction *changePasswordAct;
    QAction *intervalsAct;
    QAction *columnFormatsAct;
    QAction *protocolsAct;
    QAction *studyConfigurationsAct;
    QAction *systemSettingsAct;

    // Help menu
    QAction* epsimulatorHelpAct_;
    QAction *aboutAct;

    // menus
    QPopupMenu *studyMenu;
    QPopupMenu *catalogMenu;
    QPopupMenu* switchSubMenu_;
    QPopupMenu *utilitiesMenu;
    QPopupMenu *administrationMenu;
    QPopupMenu *securitySubMenu;
    QPopupMenu *helpMenu;

    // toolbars
    QToolBar* navigatorToolBar_;
    CatalogComboBox* catalogComboBox_;

};

#endif
