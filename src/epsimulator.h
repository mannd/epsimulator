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

class QTable;
class QAction;
class QPopupMenu;


class Epsimulator : public QMainWindow
{
    Q_OBJECT

public:
    Epsimulator();
    ~Epsimulator();

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
    QTable *qtable;

    // Study Menu
    QAction *patientInformationAct;
    QAction *consciousSedationAct;
    // other actions in this section
    QAction *ordersAct;
    QAction *reportsAct;
    QAction *exportDataAct;
    QAction *closeStudyAct;
    QAction *exitAct;   // not part of original Prucka menu

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



    QAction *aboutAct;
    
    // Main Menu Bar
    QPopupMenu *menuStudy;
    QPopupMenu *menuStudyConfiguration;
    QPopupMenu *menuMeasurements;
    QPopupMenu *menuWindows;
    QPopupMenu *menuAdministration;
    QPopupMenu *menuHelp;
};

#endif // EPSIMULATOR_H
