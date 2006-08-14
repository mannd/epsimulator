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


#ifndef EPSIMULATOR_H
#define EPSIMULATOR_H

#include <qmainwindow.h>
#include <qworkspace.h>

class QWorkspace;
class QAction;
class QPopupMenu;


class Epsimulator : public QMainWindow
{
    Q_OBJECT

public:
    Epsimulator();
    ~Epsimulator();

protected:
    //    void contextMenuEvent(QContextMenuEvent * event);

public slots:
    static void about(QWidget *);

private slots:
    void patientInformation();
    /*    void openPatient();
        void newStudy();
        void openStudy();
        // Patients and studies are saved automatically when created and modified
        // At least I think that is the Prucka behavior
        void about();
    */
    //    static void about(QWidget *);
    void closeStudy();

private:
    void createActions();
    void createMenus();



    /*    void createToolBars();
        void loadPatient();
        void savePatient();
        void loadStudy();
        void saveStudy();
    */
    // fake central widget
    QWorkspace *workspace;

    // Study Menu
    QAction *patientInformationAct;
    QAction *consciousSedationAct;
    QAction *complicationsAct;
    QAction *radiologyAct;
    QAction *medicationAct;
    QAction *suppliesAct;
    QAction *staffAct;
    QAction *clinicalProceduresAct;
    QAction *macrosAct;
    QAction *ordersAct;
    QAction *reportsAct;
    QAction *exportDataAct;
    QAction *closeStudyAct;

    // Study Configuration
    QAction *switchAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *intervalsAct;
    QAction *columnFormatsAct;
    QAction *protocolsAct;

    // Measurements
    QAction *conductionIntervalsAct;
    QAction *snrtAct;
    QAction *anteRPsAct;
    QAction *retroRPsAct;
    QAction *manualMeasurementsAct;
    QAction *statVitalsAct;
    QAction *startStopNBPAct;
    QAction *autoLogVitalsAct;
    QAction *autoLogNBPAct;
    QAction *measurementConfigurationAct;
    QAction *dataExtractionAct;

    // Windows
    QAction *winSaveAct;   // note that Study Config menu has duplicate names
    QAction *winSaveAsAct;
    QAction *winSwitchAct;
    QAction *winDeleteAct;
    QAction *timerAct;
    QAction *stopwatchAct;
    QAction *realTimeAct;
    QAction *review1Act;
    QAction *review2Act;
    QAction *logAct;
    QAction *ablationAct;
    QAction *alignmentAct;
    QAction *mapAct;
    QAction *holterAct;
    QAction *plotAct;
    QAction *macroAct;
    QAction *image1Act;
    QAction *image2Act;
    QAction *imageLibraryAct;

    // Administration Menu
    QAction *securityAct;
    QAction *systemSettingsAct;
    QAction *printSetupAct;
    QAction *adminReportsAct;   // there is a Report action in the Study menu too
    QAction *compressionRatioAct;
    QAction *amplifierTestAct;
    QAction *ejectOpticalDiskAct;


    QAction *aboutAct;
    ///TODO change format menuStudy to studyMenu throughout
    // Main Menu Bar
    QPopupMenu *menuStudy;
    QPopupMenu *menuStudyConfiguration;
    QPopupMenu *menuMeasurements;
    QPopupMenu *menuWindows;
    QPopupMenu *menuAdministration;
    QPopupMenu *menuHelp;
};

#endif // EPSIMULATOR_H
