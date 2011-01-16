/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

#ifndef PATIENTSTATUSBAR_H
#define PATIENTSTATUSBAR_H

#include "ui_patientstatusbar.h"

#include "recorderdefs.h"
#include "saturation.h"

class QMouseEvent;
class QPalette;
class QTimer;
class QWidget;

namespace EpPatient {
    class Patient;
}

namespace EpRecorder {

class PatientStatusBar : public QWidget, private Ui::PatientStatusBar {
    Q_OBJECT

public:
    PatientStatusBar(QWidget* parent = 0);
    ~PatientStatusBar();

    void start();
    void stop();

    void setPatient(EpPatient::Patient* patient) {patient_ = patient;} 
    void setPatientInfo(const QString& name, double kg, double bsa);

    void displayO2Sat();

    static void setWarningO2Sat(const EpPatient::Saturation& sat) {
        warningO2Sat_ = sat;}
    static void setDangerO2Sat(const EpPatient::Saturation& sat) {
        dangerO2Sat_ = sat;}
    static void setmalfunctionO2Sat(const EpPatient::Saturation& sat) {
        malfunctionO2Sat_ = sat;}

public slots:
    void update();
    void manualSave();
    void changeSaveStatus(SaveStatus);
    void autoSave(bool);
    void patientInformationClosed();

protected:
    void mousePressEvent(QMouseEvent*);

signals:
    void saveTriggered(SaveStatus);
    void manualSave(bool);
    void showPatientInformation();

private slots:
    void noSave();

private:
    void createPalettes();

    static const int updateInterval = 1000;
    static EpPatient::Saturation warningO2Sat_;
    static EpPatient::Saturation dangerO2Sat_;
    static EpPatient::Saturation malfunctionO2Sat_;


    EpPatient::Patient* patient_;
    QTimer* timer_;
    SaveStatus saveStatus_;
    bool autoSave_;
    QPalette defaultPalette_;
    QPalette dangerPalette_;
    QPalette warningPalette_;
};

}

#endif

