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

/** \file header file for the Navigator classs,
    which is the main window for the program.
*/

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "options.h"
#include "study.h"

#include <qfile.h>
#include <qlistview.h>
#include <qmainwindow.h>

class Options;
class QAction;
class QFrame;
class QGridLayout;
class QLabel;
class QListView;
class QPopupMenu;
class QPushButton;
class QSpacerItem;
class QSplitter;

/**
	@author David Mann <mannd@epstudiossoftware.com>
*/
class Navigator : public QMainWindow {
    Q_OBJECT

public:
    Navigator(QWidget* parent = 0, const char *name = "navigator");
    ~Navigator();

protected:
    void closeEvent(QCloseEvent * event);
    //    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    void newStudy();
    void preregisterPatient();
    void deleteStudy();
    void refreshCatalog();
    void regenerateCatalog();

    void systemSettings();

    void epsimulatorHelp();
    void about();

private:

    class TableListView : public QListView {

    public:
        TableListView(QWidget* parent);
        ~TableListView();

        bool load(const QString& fileName);
        bool save(const QString& fileName);
        void addStudy(const Study& study);

    private:
        enum {MagicNumber = 0x99c798f2};

        void readFromStream(QDataStream& in);
        void writeToStream(QDataStream& out);
        void error(const QFile& file, const QString& message);
        void ioError(const QFile& file, const QString& message);
    };

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

        Study study() {return study_;}

    private:
        Study study_;
    };

    void createCentralWidget();
    void createActions();
    void createMenus();
    void createStatusBar();

    void startStudy();
    bool getStudyInformation();
    void saveSettings();
    void readSettings();

    bool studySelected();

    void deleteDataFile();      // delete data file associated with current study
    void createDataFile();      // create a data file for a new study
    void openDataFile();        // open a previous data file for review or appending data

    Study study_;           // current study
    Options* options_;
//    QString studiesPath_;   // path to studies

    // StatusBar labels
    QLabel* messageLabel_;
    QLabel* userLabel_;
    QLabel* sourceLabel_;
    QLabel* filterLabel_;

    // central widget stuff
    QSplitter* horizontalSplitter;
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
    QAction* switchAct_;
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
    QPopupMenu *utilitiesMenu;
    QPopupMenu *administrationMenu;
    QPopupMenu *securitySubMenu;
    QPopupMenu *helpMenu;
};

#endif
