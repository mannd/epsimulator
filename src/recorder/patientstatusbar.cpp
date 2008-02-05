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

#include "study.h"

#include <QDateTime>
#include <QTimer>

PatientStatusBar::PatientStatusBar(QWidget* parent)
    : QWidget(parent) {
    setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
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
    spO2Label->setText(patient_.o2Saturation().percent());
}

void PatientStatusBar::update() {
    displayO2Sat();
    timeLabel->setText(QTime::currentTime().toString());
    dateLabel->setText(QDate::currentDate().toString());
}
    

/*$SPECIALIZATION$*/


