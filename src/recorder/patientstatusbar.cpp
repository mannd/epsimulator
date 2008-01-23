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

PatientStatusBar::PatientStatusBar(QWidget* parent, Qt::WFlags fl)
: QWidget( parent, fl ) {
    setupUi(this);
}

PatientStatusBar::~PatientStatusBar() {
}

void PatientStatusBar::setName(const Name& name) {
    nameLabel->setText(name.fullName(true));    // lastname, firstname
}

void PatientStatusBar::setO2Sat(int sat) {
    sat = sat > 100 ? 100 : sat;
    sat = sat < 0 ? 0 : sat;
    spO2Label->setText(QString::number(sat));
}
    

/*$SPECIALIZATION$*/


