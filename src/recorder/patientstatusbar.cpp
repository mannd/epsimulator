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

#include "patientstatusbar.h"

#include "patient.h"
#include "study.h"

#include <QDateTime>
#include <QTimer>
#include <QWidget>

const int PatientStatusBar::updateInterval;

Saturation PatientStatusBar::warningO2Sat_ = 90;
Saturation PatientStatusBar::dangerO2Sat_ = 80;
Saturation PatientStatusBar::malfunctionO2Sat_ = 40;

PatientStatusBar::PatientStatusBar(QWidget* parent)
    : QWidget(parent), patient_(0), saveStatus_(NoSave) {
    setupUi(this);
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_->start(updateInterval);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(manualSave()));
    // change save status when signaled by Recorder
    connect(this, SIGNAL(saveTriggered(SaveStatus)), 
        this, SLOT(changeSaveStatus(SaveStatus)));
    // saveButton also responds to signals from Recorder for autosaving
    /// TODO
    /// connect(parent, SIGNAL(saveTriggered(SaveStatus)),
    ///     this, SLOT(changeSaveStatus(SaveStatus)));
}

PatientStatusBar::~PatientStatusBar() {
}

void PatientStatusBar::setPatientInfo(const Name& name, 
    double kg, double bsa) {
    nameLabel->setText(name.fullName(true));    // lastname, firstname
    kgLabel->setText(QObject::tr("Kg ") + QString::number(kg, 'f', 1));
    bsaLabel->setText(QObject::tr("BSA ") + QString::number(bsa, 'f', 2));
}

void PatientStatusBar::displayO2Sat() {
    // here we want no reading if the probe has fallen off,
    // a black on red reading if danger, a black on yellow if
    // warning, otherwise regular text.  
    if (patient_) {
        if (patient_->o2Saturation() < malfunctionO2Sat_)
            spO2Label->setText(tr("***%"));
        else if (patient_->o2Saturation() < dangerO2Sat_)
            ; // set background/foreground
        else if (patient_->o2Saturation() < warningO2Sat_)
            ; // set warning text/backgroun
        else
            spO2Label->setText(patient_->o2Saturation().percent());
    }
}

void PatientStatusBar::update() {
    displayO2Sat();
    timeLabel->setText(QTime::currentTime().toString());
    dateLabel->setText(QDate::currentDate().toString());
}

void PatientStatusBar::hide() {
    timer_->stop();
    hide();
}

void PatientStatusBar::show() {
    timer_->start(1000);
    show();
}

void PatientStatusBar::manualSave() {
    if (saveStatus_ != ManualSave) 
        saveStatus_ = ManualSave;
    else 
        saveStatus_ = NoSave;
    emit(saveTriggered(saveStatus_));
}

void PatientStatusBar::changeSaveStatus(SaveStatus saveStatus) {
    saveStatus_ = saveStatus;
    switch (saveStatus) {
        case ManualSave: 
            saveButton->setIcon(QIcon(":/images/hi64-manualsave.png"));
            break;
        case AutoSave:
            saveButton->setIcon(QIcon(":/images/hi64-autosave.png"));
            break;
        case ExitAutoSave:
            saveButton->setIcon(QIcon(":/images/hi64-autosaveexit.png"));
            break;
        case NoSave:
        default:
            saveButton->setIcon(QIcon(":/images/hi64-nosave.png"));
    }
}
    

/*$SPECIALIZATION$*/


