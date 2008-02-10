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

#include "saturation.h"

struct Name;
class Patient;
class QTimer;
class QWidget;

class PatientStatusBar : public QWidget, private Ui::PatientStatusBar {
    Q_OBJECT
public:
    PatientStatusBar(QWidget* parent = 0);
    ~PatientStatusBar();

    void setPatient(Patient* patient) {patient_ = patient;} 
    
    void setPatientInfo(const Name&, double kg, double bsa);
    void displayO2Sat();

    virtual void hide();
    virtual void show();

    static void setWarningO2Sat(const Saturation& sat) {
        warningO2Sat_ = sat;}
    static void setDangerO2Sat(const Saturation& sat) {
        dangerO2Sat_ = sat;}
    static void setmalfunctionO2Sat(const Saturation& sat) {
        malfunctionO2Sat_ = sat;}

public slots:
    void update();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

private:
    static Saturation warningO2Sat_;
    static Saturation dangerO2Sat_;
    static Saturation malfunctionO2Sat_;

    Patient* patient_;
    QTimer* timer_;

};

#endif

